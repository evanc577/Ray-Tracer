#ifndef LIGHT_LIST_H
#define LIGHT_LIST_H

#include <vector>
#include "Light.h"

class LightList : public Light {
    public:
        LightList();
        LightList(std::vector<Light *> l);

        virtual bool AtPoint(const vec3 &p, vec3 &c, vec3 &d);

        void updateValues();
        std::vector<Light *> list;
};

#endif
