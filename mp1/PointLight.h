#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

class PointLight : public Light {
    public:
        PointLight() :
            Light(), point(vec3(0,0,0)) {}

        vec3 point;

        virtual bool AtPoint(const vec3 &p, vec3 &c, vec3 &d);
};

bool PointLight::AtPoint(const vec3 &p, vec3 &c, vec3 &d) {
    float distance = (point - p).length();
    c = color / (distance*distance) + ia;
    d = unit_vector(point - p);
    return true;
}

#endif
