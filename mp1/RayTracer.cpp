#include "RayTracer.h"

RayTracer::RayTracer() : image_(NULL) {
    setImageSize(100,100);
    antialias_ = false;
    aa_factor_ = 1;
    multithread = false;
    ortho = true;
    sRGB = false;
    sRGB_max = 1;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
}

RayTracer::RayTracer(unsigned w, unsigned h) :image_(NULL) {
    setImageSize(w,h);
    antialias_ = false;
    aa_factor_ = 1;
    multithread = false;
    ortho = true;
    sRGB = false;
    sRGB_max = 1;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
}

RayTracer::RayTracer(const RayTracer &other) {
    _copy(other);
}

RayTracer::~RayTracer() {
    _clear();
}

RayTracer & RayTracer::operator=(const RayTracer &other) {
    if (this != &other) {
        _clear();
        _copy(other);
    }
    return *this;
}

void RayTracer::outputImage(std::string filename) const {
    std::cout << "writing to " << filename << "...\n";
    if (!image_) {
        std::cout << __FUNCTION__ << "() error, no image\n";
        return;
    }

    // time start
    std::chrono::high_resolution_clock::time_point t1 =
        std::chrono::high_resolution_clock::now();

    png::image<png::rgb_pixel> output_image(image_->width_, image_->height_);
    for (png::uint_32 y = 0; y < output_image.get_height(); ++y) {
        for (png::uint_32 x = 0; x < output_image.get_width(); ++x) {
            glm::vec3 &pix = image_->getPixel(x,y);
            output_image[image_->height_-y-1][x] =
                png::rgb_pixel(255*pix[0], 255*pix[1], 255*pix[2]);
        }
    }
    output_image.write(filename);

    // time end
    std::chrono::high_resolution_clock::time_point t2 =
        std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
        ( t2 - t1 ).count();
    std::cout << "done, took " << duration << "ms\n";
}

void RayTracer::setImageSize(unsigned w, unsigned h) {
    _clear();
    image_ = new Image(w, h);
}

void RayTracer::renderSection(int thread, int num_threads) {
    float left = -1;
    float lower = -1*float(image_->height_)/float(image_->width_);

    glm::vec3 corner(left, lower, 0.0);
    glm::vec3 corner_persp(left, lower, -1.0);
    glm::vec3 horz(2.0, 0.0, 0.0);
    glm::vec3 vert(0.0, -2.0*lower, 0.0);
    glm::vec3 origin(0.0, 0.0, 0.0);
    glm::vec3 orthoDirection = corner_persp - corner;

    if (aa_factor_ < 1) {
        std::cout << "invalid antialias factor,"
            " disabling antialiasing\n";
        antialias_ = false;
    }

    for (int j = 0; j < image_->height_; j++) {
        for (int i = 0; i < image_->width_; i++) {
            if ((i+j) % num_threads != thread) continue;

            float u = float(i)/float(image_->width_);
            float v = float(j)/float(image_->height_);

            // if antialiasing is disabled
            if (!antialias_) {
                Ray r;
                float temp_u = u + 0.5/float(image_->width_);
                float temp_v = v + 0.5/float(image_->height_);
                if (ortho) {
                    r.setOrigin(corner + temp_u*horz + temp_v*vert);
                    r.setDirection(orthoDirection);
                }
                else {
                    r.setOrigin(origin);
                    r.setDirection(corner_persp + temp_u*horz + temp_v*vert);
                }
                glm::vec3 col = color(r);
                glm::vec3 &pix = image_->getPixel(i,j);
                pix = col;
            }

            // if antialiasing is enabled
            else {
                glm::vec3 col(0,0,0);
                for (int k = 0; k < aa_factor_; k++) {
                    float range_begin = float(k)/float(aa_factor_);
                    float range_end = float(k+1)/float(aa_factor_);
                    std::uniform_real_distribution<float>
                        dist(range_begin, range_end);
                    float temp_u = u + dist(generator)/float(image_->width_);
                    float temp_v = v + dist(generator)/float(image_->height_);

                    Ray r;
                    if (ortho) {
                        r.setOrigin(corner + temp_u*horz + temp_v*vert);
                        r.setDirection(orthoDirection);
                    }
                    else {
                        r.setOrigin(origin);
                        r.setDirection(corner_persp + temp_u*horz + temp_v*vert);
                    }

                    col += color(r);
                }
                float aa_float_ = float(aa_factor_);
                col /= glm::vec3(aa_float_, aa_float_, aa_float_);
                glm::vec3 &pix = image_->getPixel(i,j);
                pix = col;
            }
        }
    }
}

void RayTracer::render() {
    std::cout << "rendering image...\n";
    if (!image_) {
        std::cout << __FUNCTION__ << "() error, image size not set\n";
        return;
    }

    lights.updateValues();

    // time start
    std::chrono::high_resolution_clock::time_point t1 =
        std::chrono::high_resolution_clock::now();

    if (multithread) {
        int num_threads = std::thread::hardware_concurrency();
        if (num_threads < 1) num_threads = 1;
        std::vector<std::thread> threads(num_threads);

        int thread = 0;
        for (std::vector<std::thread>::iterator it = threads.begin();
                it != threads.end(); ++it) {
            (*it) = std::thread([=] { renderSection(thread, num_threads); });
            thread++;
        }
        for (std::vector<std::thread>::iterator it = threads.begin();
                it != threads.end(); ++it) {
            (*it).join();
        }
    }

    // if multithread is disabled
    else {
        renderSection(0, 1);
    }

    // transform to sRGB
    if (sRGB) linearToSRGB();

    // scale values to avoid exceeding 1
    if (sRGB){
        if (sRGB_max > 1) {
            for (int i = 0; i < image_->width_; i++) {
                for (int j = 0; j < image_->height_; j++) {
                    glm::vec3 & p = image_->getPixel(i,j);
                    p /= sRGB_max;
                }
            }
        }
    }
    else {
        float max_val = 0;
        for (int i = 0; i < image_->width_; i++) {
            for (int j = 0; j < image_->height_; j++) {
                glm::vec3 & p = image_->getPixel(i,j);
                if (p[0] > max_val) max_val = p[0];
                if (p[1] > max_val) max_val = p[1];
                if (p[2] > max_val) max_val = p[2];
            }
        }

        if (max_val > 1) {
            for (int i = 0; i < image_->width_; i++) {
                for (int j = 0; j < image_->height_; j++) {
                    glm::vec3 & p = image_->getPixel(i,j);
                    p /= max_val;
                }
            }
        }
    }

    // time end
    std::chrono::high_resolution_clock::time_point t2 =
        std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
        ( t2 - t1 ).count();
    std::cout << "done, took " << duration << "ms\n";
}

glm::vec3 RayTracer::color(const Ray &r) {
    hit_record rec;
    if (hittables.hit(r,0.0,std::numeric_limits<float>::max(),rec, lights)) {
        return hittables.color(rec, lights, r.direction());
    }
    else {
        return glm::vec3(0,0,0);
    }
}

void RayTracer::linearToSRGB() {
    float c = 0.0031308;
    float a = 0.055;

    for (int j = 0; j < image_->height_; j++) {
        for (int i = 0; i < image_->width_; i++) {
            glm::vec3 &pix = image_->getPixel(i,j);
            for (int k = 0; k < 3; k++) {
                if (pix[k] <= c) pix[k] *= 12.92;
                else pix[k] = (1+a)*pow(pix[k], 1/2.4) - a;

                if (pix[k] > sRGB_max) sRGB_max = pix[k];
            }
        }
    }
}

void RayTracer::addHittable(Hittable *h) {
    hittables.list_.push_back(h);
}

void RayTracer::clearHittables() {
    hittables.list_.clear();
}

void RayTracer::addLight(Light *l) {
    lights.list.push_back(l);

}

void RayTracer::_clear() {
    if (image_) {
        delete image_;
        image_ = NULL;
    }
}

void RayTracer::_copy(const RayTracer &other) {
    image_ = other.image_;
}
