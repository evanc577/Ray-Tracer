#include "AmbientLight.h"

AmbientLight::AmbientLight() : Light() {}

bool AmbientLight::AtPoint(const glm::vec3 &p, std::vector<glm::vec3> &ca,
        std::vector<glm::vec3> &cd, std::vector<glm::vec3> &cs,
        std::vector<glm::vec3> &d) {
    (void)p;
    (void)d;

    ca.clear();
    cd.clear();
    cs.clear();
    d.clear();

    ca.push_back(ia);
    cd.push_back(glm::vec3(0,0,0));
    cs.push_back(glm::vec3(0,0,0));
    d.push_back(glm::vec3(0,0,0));
    return true;
}
