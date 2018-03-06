#include "AmbientLight.h"

AmbientLight::AmbientLight() : Light() {}

bool AmbientLight::AtPoint(const glm::vec3 &p, glm::vec3 &c, glm::vec3 &d) {
    (void)p;
    (void)d;
    c = ia;
    return false;
}
