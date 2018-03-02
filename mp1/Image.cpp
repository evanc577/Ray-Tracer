#include "Image.h"

Image::Image() : width_(0), height_(0) {}

Image::Image(unsigned w, unsigned h) : width_(w), height_(h) {
    image = new std::vector<std::vector<Pixel>>((w),std::vector<Pixel>(h));
}

bool Image::setPixel(unsigned x, unsigned y, const Pixel &p) {
    if (x >= width_ || y >= height_) return false;
    image->at(x).at(y) = p;
    return true;
}

Pixel& Image::getPixel(unsigned x, unsigned y) const {
    return image->at(x).at(y);
}

Image::~Image() {
    _clear();
}

void Image::_clear() {
    delete image;
}
