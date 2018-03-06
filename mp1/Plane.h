#ifndef PLANE_H
#define PLANE_H

#include "Hittable.h"

class Plane : public Hittable {
    public:
        Plane();
        Plane(glm::vec3 p, glm::vec3 n);
        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec, Light &l) const;

        glm::vec3 point;
        glm::vec3 normal;
};

#endif
