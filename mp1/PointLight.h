#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

class PointLight : public Light {
    public:
        PointLight() :
            Light(), point(glm::vec3(0,0,0)) {}

        glm::vec3 point;

        virtual bool AtPoint(const glm::vec3 &p, glm::vec3 &c, glm::vec3 &d);
};

bool PointLight::AtPoint(const glm::vec3 &p, glm::vec3 &c, glm::vec3 &d) {
    float distance = (point - p).length();
    c = color / (distance*distance) + ia;
    d = glm::normalize(point - p);
    return true;
}

#endif
