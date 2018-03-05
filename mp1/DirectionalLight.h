#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight : public Light {
    public:
        DirectionalLight() : Light() {
            direction = unit_vector(vec3(0,0,-1));
        }

        vec3 direction;

        virtual bool AtPoint(const vec3 &p, vec3 &c, vec3 &d);
};

bool DirectionalLight::AtPoint(const vec3 &p, vec3 &c, vec3 &d) {
    (void)p;
    c = color + ia;
    d = -1.0*direction;
    return true;
}

#endif
