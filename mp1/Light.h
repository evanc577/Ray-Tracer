#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include "vec3.h"

class Light {
    public:
        Light() : color(vec3(1,1,1)), ia(vec3(0,0,0)) {}

        vec3 color;

        vec3 ia;

        virtual bool AtPoint(const vec3 &p, vec3 &c, vec3 &d) = 0;
};

#endif
