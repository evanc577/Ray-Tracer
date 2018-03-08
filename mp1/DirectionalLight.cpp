#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
    direction = glm::normalize(glm::vec3(0,0,-1));
}

bool DirectionalLight::AtPoint(const glm::vec3 &p, glm::vec3 &ca,
        glm::vec3 &cd, glm::vec3 &cs, glm::vec3 &d) {
    (void)p;
    ca = ia;
    cd = id;
    cs = is;
    d = -1.0f*direction;
    return true;
}
