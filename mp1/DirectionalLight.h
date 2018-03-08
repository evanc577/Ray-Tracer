#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight : public Light {
    public:
        DirectionalLight();

        glm::vec3 direction;

        virtual bool AtPoint(const glm::vec3 &p, glm::vec3 &ca,
                glm::vec3 &cd, glm::vec3 &cs, glm::vec3 &d);
};

#endif
