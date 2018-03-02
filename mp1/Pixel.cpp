#include "Pixel.h"

Pixel::Pixel() : r(0), g(0), b(0), a(255) {}

Pixel::Pixel(float red, float green, float blue, float alpha) :
    r(red), g(green), b(blue), a(alpha) {}
