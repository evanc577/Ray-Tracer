#ifndef HITTABLE_H
#define HITTABLE_H

#include "Light.h"
#include "Ray.h"

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;

    vec3 ka;

    vec3 kd;
    vec3 l;

    vec3 ks;
    vec3 r;
    vec3 alpha;
};

class Hittable {
    public:
        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec, Light &l) const = 0;

        virtual ~Hittable() {}

        vec3 ka = vec3(1,1,1);
        vec3 kd = vec3(1,1,1);
        vec3 ks;
        vec3 alpha;
};
#endif
