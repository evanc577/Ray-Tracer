#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Hittable.h"

class Triangle : public Hittable {
    public:
        Triangle();
        Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
        
        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec, Light &l) const;

        glm::vec3 A;
        glm::vec3 B;
        glm::vec3 C;

};
#endif
