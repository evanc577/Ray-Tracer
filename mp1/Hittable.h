#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    vec3 color;
};

class Hittable {
    public:
        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec) const = 0;

        virtual ~Hittable() {}
};
#endif
