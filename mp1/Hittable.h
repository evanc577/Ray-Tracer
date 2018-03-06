#ifndef HITTABLE_H
#define HITTABLE_H

#include "Light.h"
#include "Ray.h"

struct hit_record {
    float t;
    glm::vec3 p;
    glm::vec3 normal;

    glm::vec3 ka;

    glm::vec3 kd;
    glm::vec3 l;

    glm::vec3 ks;
    glm::vec3 r;
    glm::vec3 alpha;
};

class Hittable {
    public:
        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec, Light &l) const = 0;

        virtual ~Hittable() {}

        glm::vec3 ka = glm::vec3(1,1,1);
        glm::vec3 kd = glm::vec3(1,1,1);
        glm::vec3 ks;
        glm::vec3 alpha;
};
#endif
