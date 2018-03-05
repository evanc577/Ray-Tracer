#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight : public Light {
    public:
        DirectionalLight();

        vec3 direction;

        virtual bool AtPoint(const vec3 &p, vec3 &c, vec3 &d);
};

#endif
