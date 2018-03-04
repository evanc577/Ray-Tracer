#include <chrono>
#include <random>
#include <limits>
#include "RayTracer.h"

RayTracer::RayTracer() : image_(NULL) {
    setImageSize(100,100);
    antialias_ = false;
    aa_factor_ = 1;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
}

RayTracer::RayTracer(unsigned w, unsigned h) :image_(NULL) {
    setImageSize(w,h);
    antialias_ = false;
    aa_factor_ = 1;

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
    if (!image_) {
        std::cout << __FUNCTION__ << "() error, no image\n";
        return;
    }

    std::vector<unsigned char> temp;
    temp.resize(image_->width_ * image_->height_ * 4);
    for (int i = 0; i < image_->width_; i++) {
        for (int j = 0; j < image_->height_; j++) {
            vec3 &p = image_->getPixel(i,j);
            temp[4*(image_->height_-j-1)*image_->width_ + 4*i + 0] =
                (unsigned char)(255*p.r());
            temp[4*(image_->height_-j-1)*image_->width_ + 4*i + 1] =
                (unsigned char)(255*p.g());
            temp[4*(image_->height_-j-1)*image_->width_ + 4*i + 2] =
                (unsigned char)(255*p.b());
            temp[4*(image_->height_-j-1)*image_->width_ + 4*i + 3] =
                (unsigned char)(255);
        }
    }

    unsigned error = lodepng::encode(filename, temp, image_->width_,
            image_->height_);
    if (error) {
        std::cout << "encoder error " << error << ": "<<
            lodepng_error_text(error) << std::endl;
    }
}

void RayTracer::setImageSize(unsigned w, unsigned h) {
    _clear();
    image_ = new Image(w, h);
}

void RayTracer::render(bool ortho) {
    if (!image_) {
        std::cout << __FUNCTION__ << "() error, image size not set\n";
        return;
    }


    float left = -1;
    float lower = -1*float(image_->height_)/float(image_->width_);

    vec3 corner(left, lower, 0.0);
    vec3 corner_persp(left, lower, -1.0);
    vec3 horz(2.0, 0.0, 0.0);
    vec3 vert(0.0, -2.0*lower, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    vec3 orthoDirection = corner_persp - corner;

    if (aa_factor_ < 1) {
        std::cout << "invalid antialias factor,"
            " disabling antialiasing\n";
        antialias_ = false;
    }

    for (int j = 0; j < image_->height_; j++) {
        for (int i = 0; i < image_->width_; i++) {
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
                vec3 col = color(r);
                vec3 &pix = image_->getPixel(i,j);
                pix = col;
            }

            // if antialiasing is enabled
            else {
                vec3 col(0,0,0);
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
                col /= vec3(aa_float_, aa_float_, aa_float_);
                vec3 &pix = image_->getPixel(i,j);
                pix = col;
            }
        }
    }
}

vec3 RayTracer::color(const Ray &r) {
    hit_record rec;
    if (hittables.hit(r,0.0,std::numeric_limits<float>::max(),rec)) {
        return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
    }
    else {
        return vec3(0,0,0);
    }
}

void RayTracer::addHittable(Hittable *h) {
    hittables.list_.push_back(h);
}

void RayTracer::clearHittables() {
    hittables.list_.clear();
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
