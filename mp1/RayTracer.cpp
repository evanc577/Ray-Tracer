#include "RayTracer.h"

RayTracer::RayTracer() : image(NULL) {
    //TODO implement ctor
}

RayTracer::RayTracer(const RayTracer &other) {
    //TODO implement copy ctor
    _copy(other);
}

RayTracer::~RayTracer() {
    //TODO implement dtor
    _clear();
}

RayTracer & RayTracer::operator=(const RayTracer &other) {
    //TODO implement assignment operator

    if (this != &other) {
        _clear();
        _copy(other);
    }
    return *this;
}

void RayTracer::outputImage(std::string filename) const {
    //TODO implement outputImage
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
    for (unsigned i = 0; i < image->width_; i++) {
        for (unsigned j = 0; j < image->height_; j++) {
            Pixel &p = image->getPixel(i, j);
            p.r = float(i)/image->width_;
            p.g = float(j)/image->height_;
            p.b = .2;
            p.a = 1;
        }
    }
}

void RayTracer::_clear() {
    //TODO implement _clear()
    if (image) {
        delete image;
        image = NULL;
    }
}

void RayTracer::_copy(const RayTracer &other) {
    //TODO implement _copy()
    (void)other;
}
