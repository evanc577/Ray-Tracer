#include "RayTracer.h"

RayTracer::RayTracer() : image(NULL) {
    setImageSize(100,100);
}

RayTracer::RayTracer(unsigned w, unsigned h) :image(NULL) {
    setImageSize(w,h);
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
    if (!image) {
        std::cout << __FUNCTION__ << "() error, no image\n";
        return;
    }

    std::vector<unsigned char> temp;
    temp.resize(image->width_ * image->height_ * 4);
    for (unsigned i = 0; i < image->width_; i++) {
        for (unsigned j = 0; j < image->height_; j++) {
            temp[4*j*image->width_ + 4*i + 0] = 255*image->getPixel(i,j).r;
            temp[4*j*image->width_ + 4*i + 1] = 255*image->getPixel(i,j).g;
            temp[4*j*image->width_ + 4*i + 2] = 255*image->getPixel(i,j).b;
            temp[4*j*image->width_ + 4*i + 3] = 255*image->getPixel(i,j).a;
        }
    }

    unsigned error = lodepng::encode(filename, temp, image->width_,
            image->height_);
    if (error) {
        std::cout << "encoder error " << error << ": "<<
            lodepng_error_text(error) << std::endl;
    }
}

void RayTracer::test() const {
    std::cout << "test" << std::endl;
}

void RayTracer::setImageSize(unsigned w, unsigned h) {
    _clear();
    image = new Image(w, h);
}

void RayTracer::render() {
    //TODO implement render()
    if (!image) {
        std::cout << __FUNCTION__ << "() error, image size not set\n";
        return;
    }

    auto start = std::chrono::high_resolution_clock::now();

    for (unsigned i = 0; i < image->height_; i++) {
        for (unsigned j = 0; j < image->width_; j++) {
            Pixel &p = image->getPixel(i, j);
            p.r = float(i)/image->width_;
            p.g = float(j)/image->height_;
            p.b = .2;
            p.a = 1;
        }
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count() << "ms\n";

}

void RayTracer::_clear() {
    if (image) {
        delete image;
        image = NULL;
    }
}

void RayTracer::_copy(const RayTracer &other) {
    image = other.image;
}
