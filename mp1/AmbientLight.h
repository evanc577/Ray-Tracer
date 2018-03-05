#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "Light.h"

class AmbientLight : public Light {
    public:
        AmbientLight() : Light() {}

        virtual bool AtPoint(const vec3 &p, vec3 &c, vec3 &d);
};

bool AmbientLight::AtPoint(const vec3 &p, vec3 &c, vec3 &d) {
    (void)p;
    (void)d;
    c = ia;
    return false;
}
#endif
