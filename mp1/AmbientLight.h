#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "Light.h"

class AmbientLight : public Light {
    public:
        AmbientLight();

        virtual bool AtPoint(const glm::vec3 &p, glm::vec3 &ca,
                glm::vec3 &cd, glm::vec3 &cs, glm::vec3 &d);
};

#endif
