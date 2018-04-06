#include "RayTracer.h"

RayTracer::RayTracer() : image_(NULL) {
  setImageSize(100, 100);
  antialias_ = false;
  aa_factor_ = 1;
  multithread = false;
  ortho = true;
  BVH = true;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
}

RayTracer::RayTracer(unsigned w, unsigned h) : image_(NULL) {
  setImageSize(w, h);
  antialias_ = false;
  aa_factor_ = 1;
  multithread = false;
  ortho = true;
  BVH = true;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
}

RayTracer::RayTracer(const RayTracer &other) { _copy(other); }

RayTracer::~RayTracer() { _clear(); }

RayTracer &RayTracer::operator=(const RayTracer &other) {
  if (this != &other) {
    _clear();
    _copy(other);
  }
  return *this;
}

void RayTracer::outputImage(std::string filename) const {
  std::cout << "writing to " << filename << "..." << std::endl;
  if (!image_) {
    std::cout << __FUNCTION__ << "() error, no image" << std::endl;
    return;
  }

  // time start
  std::chrono::high_resolution_clock::time_point t1 =
      std::chrono::high_resolution_clock::now();

  png::image<png::rgb_pixel> output_image(image_->width_, image_->height_);
  for (png::uint_32 y = 0; y < output_image.get_height(); ++y) {
    for (png::uint_32 x = 0; x < output_image.get_width(); ++x) {
      vec3 &pix = image_->getPixel(x, y);
      output_image[image_->height_ - y - 1][x] =
          png::rgb_pixel(255 * pix[0], 255 * pix[1], 255 * pix[2]);
    }
  }
  output_image.write(filename);

  // time end
  std::chrono::high_resolution_clock::time_point t2 =
      std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  std::cout << "done, took " << duration << "ms" << std::endl;
}

void RayTracer::setImageSize(unsigned w, unsigned h) {
  _clear();
  image_ = new Image(w, h);
}

void RayTracer::renderSection(int thread, int num_threads) {
  float left = -1;
  float lower = -1 * float(image_->height_) / float(image_->width_);

  vec3 corner(left, lower, 0.0);
  vec3 corner_persp(left, lower, -1.0);
  vec3 horz(2.0, 0.0, 0.0);
  vec3 vert(0.0, -2.0 * lower, 0.0);
  vec3 origin(0.0, 0.0, 0.0);
  vec3 orthoDirection = corner_persp - corner;

  if (aa_factor_ < 1) {
    std::cout << "invalid antialias factor,"
                 " disabling antialiasing" << std::endl;
    antialias_ = false;
  }

  for (int j = 0; j < image_->height_; j++) {
    for (int i = 0; i < image_->width_; i++) {
      if ((i + j) % num_threads != thread) continue;

      float u = float(i) / float(image_->width_);
      float v = float(j) / float(image_->height_);

      // if antialiasing is disabled
      if (!antialias_) {
        Ray r;
        float temp_u = u + 0.5 / float(image_->width_);
        float temp_v = v + 0.5 / float(image_->height_);
        if (ortho) {
          r.setOrigin(corner + temp_u * horz + temp_v * vert);
          r.setDirection(orthoDirection);
        } else {
          r.setOrigin(origin);
          r.setDirection(corner_persp + temp_u * horz + temp_v * vert);
        }
        vec3 col = color(r);
        vec3 &pix = image_->getPixel(i, j);
        pix = col;
      }

      // if antialiasing is enabled
      else {
        vec3 col(0, 0, 0);
        for (int k = 0; k < aa_factor_; k++) {
          float range_begin = float(k) / float(aa_factor_);
          float range_end = float(k + 1) / float(aa_factor_);
          std::uniform_real_distribution<float> dist(range_begin, range_end);
          float temp_u = u + dist(generator) / float(image_->width_);
          float temp_v = v + dist(generator) / float(image_->height_);

          Ray r;
          if (ortho) {
            r.setOrigin(corner + temp_u * horz + temp_v * vert);
            r.setDirection(orthoDirection);
          } else {
            r.setOrigin(origin);
            r.setDirection(corner_persp + temp_u * horz + temp_v * vert);
          }

          col += color(r);
        }
        float aa_float_ = float(aa_factor_);
        col /= vec3(aa_float_, aa_float_, aa_float_);
        vec3 &pix = image_->getPixel(i, j);
        pix = col;
      }
    }
  }
}

void RayTracer::render() {
  if (!image_) {
    std::cout << __FUNCTION__ << "() error, image size not set" << std::endl;
    return;
  }

  if (BVH) {
    std::cout << "generating BVH..." << std::endl;
    // time start
    std::chrono::high_resolution_clock::time_point t1 =
      std::chrono::high_resolution_clock::now();

    hittables_BVH.generate();

    // time end
    std::chrono::high_resolution_clock::time_point t2 =
      std::chrono::high_resolution_clock::now();
    auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
    std::cout << "done, took " << duration << "ms" << std::endl;
  } else {
    hittables.generate();
  }

  std::cout << "rendering image..." << std::endl;

  // time start
  std::chrono::high_resolution_clock::time_point t1 =
      std::chrono::high_resolution_clock::now();

  if (multithread) {
    int num_threads = std::thread::hardware_concurrency();
    if (num_threads < 1) num_threads = 1;
    std::vector<std::thread> threads(num_threads);

    int thread = 0;
    for (auto& t : threads) {
      t = std::thread([=] { renderSection(thread, num_threads); });
      thread++;
    }
    for (auto& t : threads) {
      t.join();
    }
  }

  // if multithread is disabled
  else {
    renderSection(0, 1);
  }

  // scale values to avoid exceeding 1
  float gamma = 0.9f;
  if (BVH) {
    float A = pow(hittables_BVH.max_val, -gamma);
    if (hittables_BVH.max_val > 1) {
      for (int i = 0; i < image_->width_; i++) {
        for (int j = 0; j < image_->height_; j++) {
          vec3 &p = image_->getPixel(i, j);
          for (int k = 0; k < 3; k++) {
            p[k] = A * pow(p[k], gamma);
          }
        }
      }
    }
  } else {
    float A = pow(hittables.max_val, -gamma);
    if (hittables.max_val > 1) {
      for (int i = 0; i < image_->width_; i++) {
        for (int j = 0; j < image_->height_; j++) {
          vec3 &p = image_->getPixel(i, j);
          for (int k = 0; k < 3; k++) {
            p[k] = A * pow(p[k], gamma);
          }
        }
      }
    }
  }

  // time end
  std::chrono::high_resolution_clock::time_point t2 =
      std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  std::cout << "done, took " << duration << "ms" << std::endl;
}

vec3 RayTracer::color(const Ray &r) {
  hit_record rec;
  if (BVH) {
    if (hittables_BVH.hit(r, 0.0, std::numeric_limits<float>::max(), rec, lights)) {
      return hittables_BVH.color(rec, lights, r.direction());
    } else {
      return vec3(0, 0, 0);
    } 
  } else {
    if (hittables.hit(r, 0.0, std::numeric_limits<float>::max(), rec, lights)) {
      return hittables.color(rec, lights, r.direction());
    } else {
      return vec3(0, 0, 0);
    }
  }
}

void RayTracer::addHittable(Hittable *h) { 
  if (BVH) {
    hittables_BVH.add_hittable(h);
  } else {
    hittables.add_hittable(h);
  }
}

void RayTracer::clearHittables() { 
  if (BVH) {
    hittables_BVH.list_.clear();
    hittables_BVH.unbounded_list_.clear();
    hittables_BVH.tree_.clear();
  } else {
    hittables.list_.clear();
  }
}

void RayTracer::addLight(Light *l) { lights.list.push_back(l); }

void RayTracer::_clear() {
  if (image_) {
    delete image_;
    image_ = NULL;
  }
}


void RayTracer::_copy(const RayTracer &other) { image_ = other.image_; }
