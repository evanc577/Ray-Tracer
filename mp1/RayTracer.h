#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include "lodepng/lodepng.h"
#include "Image.h"

class RayTracer {
    public:
        RayTracer();
        RayTracer(unsigned w, unsigned h);
        RayTracer(const RayTracer &other);
        ~RayTracer();
        RayTracer & operator=(const RayTracer &other);

        void outputImage(std::string filename) const;

        void test() const;
        void setImage(const Image &im);
        void setImageSize(unsigned w, unsigned h);

        Image * image;

        void render();

    private:
        void _copy(const RayTracer &other);
        void _clear();
};

#endif
