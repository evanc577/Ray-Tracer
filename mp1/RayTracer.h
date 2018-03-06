#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <thread>
#include <png++/png.hpp>
#include "Image.h"
#include "glm/glm/glm.hpp"
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

        bool sRGB;

    private:
        void _copy(const RayTracer &other);
        void _clear();

        void renderSection(int thread, int num_threads);

        void linearToSRGB();
        float sRGB_max;

        glm::vec3 color(const Ray &r);
        glm::vec3 randomInUnitSphere();

        HittableList hittables;
        LightList lights;

        std::default_random_engine generator;
};

#endif
