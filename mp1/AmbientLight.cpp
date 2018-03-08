#include "AmbientLight.h"

AmbientLight::AmbientLight() : Light() {}

bool AmbientLight::AtPoint(const glm::vec3 &p, glm::vec3 &ca,
        glm::vec3 &cd, glm::vec3 &cs, glm::vec3 &d) {
    (void)p;
    (void)d;
    ca = ia;
    cd = glm::vec3(0,0,0);
    cs = glm::vec3(0,0,0);
    return true;
}
