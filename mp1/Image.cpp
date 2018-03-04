#include "Image.h"

Image::Image() : width_(0), height_(0) {}

Image::Image(int w, int h) : width_(w), height_(h) {
    image = new std::vector<std::vector<vec3>>((w),std::vector<vec3>(h));
}

bool Image::setPixel(int x, int y, const vec3 &p) {
    if (x >= width_ || y >= height_) return false;
    image->at(x).at(y) = p;
    return true;
}

vec3& Image::getPixel(int x, int y) const {
    return image->at(x).at(y);
}

Image::~Image() {
    _clear();
}

void Image::_clear() {
    delete image;
}
