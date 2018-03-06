#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
    direction = glm::normalize(glm::vec3(0,0,-1));
}

bool DirectionalLight::AtPoint(const glm::vec3 &p, glm::vec3 &c, glm::vec3 &d) {
    (void)p;
    c = color + ia;
    d = -1.0f*direction;
    return true;
}
