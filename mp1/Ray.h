#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
    public:
        Ray();
        Ray(const vec3 &a, const vec3 &b);
        vec3 origin() const;
        vec3 direction() const;
        vec3 location(float t) const;

    private:
        vec3 A_;
        vec3 B_;
};

#endif
