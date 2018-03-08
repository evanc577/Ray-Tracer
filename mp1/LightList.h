#ifndef LIGHT_LIST_H
#define LIGHT_LIST_H

#include <vector>
#include "Light.h"

class LightList : public Light {
    public:
        LightList();
        LightList(std::vector<Light *> l);

        virtual bool AtPoint(const glm::vec3 &p, glm::vec3 &ca,
                glm::vec3 &cd, glm::vec3 &cs, glm::vec3 &d);

        void updateValues();
        std::vector<Light *> list;
};

#endif
