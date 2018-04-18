#include "RayTracer.h"

RayTracer::RayTracer() : image_(nullptr), triangles(nullptr) {
  setImageSize(100, 100);
  antialias_ = false;
  aa_factor_ = 1;
  multithread = false;
  ortho = true;
  BVH = true;

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine generator(seed);
}

RayTracer::RayTracer(unsigned w, unsigned h) : image_(nullptr), triangles(nullptr) {
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
  std::cout << "    ok, took " << duration << "ms" << std::endl;
}

void RayTracer::setImageSize(unsigned w, unsigned h) {
  _clear();
  image_ = new Image(w, h);
}

void RayTracer::renderSection(int thread, int num_threads) {
  if (aa_factor_ < 1) {
    std::cout << "invalid antialias factor,"
                 " disabling antialiasing"
              << std::endl;
    antialias_ = false;
  }

  // for progress indicator
  int total_pixels;
  int last_percentage;
  if (thread == 0) {
    total_pixels = image_->width_ * image_->height_;
    last_percentage = -1;
  }

  for (int j = 0; j < image_->height_; ++j) {
    for (int i = 0; i < image_->width_; ++i) {
      if ((i + j) % num_threads != thread) continue;

      // update progress percentage
      if (thread == 0) {
        int percentage = 100 * (float(j * image_->width_ + i) / total_pixels);
        if (percentage > last_percentage) {
          std::string str =
              "rendering image..." + std::to_string(percentage) + "%\r";
          std::cout << str << std::flush;
          last_percentage = percentage;
        }
      }

      float u = float(i) / float(image_->width_);
      float v = float(j) / float(image_->height_);

      // if antialiasing is disabled
      if (!antialias_) {
        Ray r;
        float temp_u = u + 0.5 / float(image_->width_);
        float temp_v = v + 0.5 / float(image_->height_);
        if (ortho) {
          r = o_cam.get_ray(temp_u, temp_v);
        } else {
          r = p_cam.get_ray(temp_u, temp_v);
        }
        vec3 col = color(r);
        vec3 &pix = image_->getPixel(i, j);
        pix = col;
      }

      // if antialiasing is enabled
      else {
        vec3 col(0, 0, 0);
        for (int k = 0; k < aa_factor_; ++k) {
          for (int l = 0; l < aa_factor_; ++l) {
            float x_range_begin = float(k) / float(aa_factor_);
            float x_range_end = float(k + 1) / float(aa_factor_);
            float y_range_begin = float(l) / float(aa_factor_);
            float y_range_end = float(l + 1) / float(aa_factor_);
            std::uniform_real_distribution<float> x_dist(x_range_begin,
                                                         x_range_end);
            std::uniform_real_distribution<float> y_dist(y_range_begin,
                                                         y_range_end);
            float temp_u = u + x_dist(generator) / float(image_->width_);
            float temp_v = v + y_dist(generator) / float(image_->height_);

            Ray r;
            if (ortho) {
              r = o_cam.get_ray(temp_u, temp_v);
            } else {
              r = p_cam.get_ray(temp_u, temp_v);
            }

            col += color(r);
          }
        }
        float aa_float_ = float(aa_factor_ * aa_factor_);
        col /= vec3(aa_float_, aa_float_, aa_float_);
        vec3 &pix = image_->getPixel(i, j);
        pix = col;
      }
    }
  }
  // finish progress indicator
  if (thread == 0) {
    std::cout << std::endl;
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
    std::cout << "    ok, took " << duration << "ms" << std::endl;
  } else {
    hittables.generate();
  }

  // time start
  std::chrono::high_resolution_clock::time_point t1 =
      std::chrono::high_resolution_clock::now();

  if (multithread) {
    int num_threads = std::thread::hardware_concurrency();
    if (num_threads < 1) num_threads = 1;
    std::vector<std::thread> threads(num_threads);

    int thread = 0;
    for (auto &t : threads) {
      t = std::thread([=] { renderSection(thread, num_threads); });
      ++thread;
    }
    for (auto &t : threads) {
      t.join();
    }
  }

  // if multithread is disabled
  else {
    renderSection(0, 1);
  }

  // scale values to avoid exceeding 1
  float gamma = 0.9f;
  float max_val;
  if (BVH) {
    max_val = hittables_BVH.max_val;
  } else {
    max_val = hittables.max_val;
  }
  float A = pow(max_val, -gamma);
  if (max_val > 1) {
    for (int i = 0; i < image_->width_; ++i) {
      for (int j = 0; j < image_->height_; ++j) {
        vec3 &p = image_->getPixel(i, j);
        p[0] = A * pow(p[0], gamma);
        p[1] = A * pow(p[1], gamma);
        p[2] = A * pow(p[2], gamma);
      }
    }
  }
  // time end
  std::chrono::high_resolution_clock::time_point t2 =
      std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  std::cout << "    ok, took " << duration << "ms" << std::endl;
}

vec3 RayTracer::color(const Ray &r) {
  hit_record rec;
  if (BVH) {
    if (hittables_BVH.hit(r, 0.0, std::numeric_limits<float>::max(), rec,
                          lights)) {
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
    image_ = nullptr;
  }
  if (triangles) {
    delete triangles;
    triangles = nullptr;
  }
}

void RayTracer::_copy(const RayTracer &other) { image_ = other.image_; }

void RayTracer::set_ortho_cam(vec3 origin, vec3 direction, vec3 vup,
                              float width, float height) {
  o_cam.set_camera(origin, direction, vup, width, height);
}

void RayTracer::set_persp_cam(vec3 origin, vec3 direction, vec3 vup,
                              float aspect, float hfov) {
  p_cam.set_camera(origin, direction, vup, aspect, hfov);
}

void RayTracer::read_file(const std::string &filename) {
  std::cout << "reading file..." << std::endl;
  std::ifstream infile;
  infile.open(filename);
  std::vector<vec3> vertices;
  triangles = new std::vector<Triangle>;
  std::string line;

  while (std::getline(infile, line)) {
    if (line[0] == '#') {
      std::cout << "comment" << std::endl;
      continue;
    }
    std::istringstream iss(line);
    std::string type, a, b, c;
    if (!(iss >> type >> a >> b >> c)) {
      std::cout << "ERROR" << std::endl;
    }
    if (type == "v") {
      float x = std::stof(a);
      float y = std::stof(b);
      float z = std::stof(c);
      vertices.push_back(vec3(x, y, z));
    } else if (type == "f") {
      int x = std::stoi(a);
      int y = std::stoi(b);
      int z = std::stoi(c);
      Triangle tri(vertices[x - 1], vertices[y - 1], vertices[z - 1]);
      triangles->push_back(tri);
    }
  }
  for (Triangle& t : *triangles) {
    addHittable(&t);
  }
  std::cout << "    ok" << std::endl;
  std::cout << "    " <<  vertices.size() << " vertices" << std::endl;
  std::cout << "    " <<  triangles->size() << " triangles" << std::endl;
}
