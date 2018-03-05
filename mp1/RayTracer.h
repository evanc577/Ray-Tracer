#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <thread>
#include "lodepng/lodepng.h"
#include "Image.h"
#include "vec3.h"
#include "Ray.h"
#include "HittableList.h"
#include "LightList.h"

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

        void addHittable(Hittable *h);
        void clearHittables();

        void addLight(Light *l);
        void clearLights();

        Image * image_;
        void render();

        bool ortho;

        bool antialias_;
        int aa_factor_;

        bool multithread;

    private:
        void _copy(const RayTracer &other);
        void _clear();

        void renderSection(int start_row, int end_row);

        vec3 color(const Ray &r);
        vec3 randomInUnitSphere();

        HittableList hittables;
        LightList lights;

        std::default_random_engine generator;
};

#endif
