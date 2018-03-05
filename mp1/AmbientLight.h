#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "Light.h"

class AmbientLight : public Light {
    public:
        AmbientLight();

        virtual bool AtPoint(const vec3 &p, vec3 &c, vec3 &d);
};

#endif
