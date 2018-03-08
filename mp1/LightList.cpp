#include "LightList.h"

LightList::LightList() : Light() {}

LightList::LightList(std::vector<Light *> l) : Light() {list = l;}

void LightList::updateValues() {
    for (std::vector<Light *>::const_iterator it = list.begin();
            it != list.end(); ++it) {
        ia += (*it)->ia;
    }
}

bool LightList::AtPoint(const glm::vec3 &p, glm::vec3 &ca,
        glm::vec3 &cd, glm::vec3 &cs, glm::vec3 &d) {
    ca = glm::vec3(0,0,0);
    cd = glm::vec3(0,0,0);
    cs = glm::vec3(0,0,0);
    d = glm::vec3(0,0,0);
    glm::vec3 temp_ca(0,0,0);
    glm::vec3 temp_cd(0,0,0);
    glm::vec3 temp_cs(0,0,0);

    glm::vec3 temp_d(0,0,0);
    bool directional = false;
    for (std::vector<Light *>::const_iterator it = list.begin();
            it != list.end(); ++it) {
        if ((*it)->AtPoint(p, temp_ca, temp_cd, temp_cs, temp_d)) {
            directional = true;
            d += temp_d;
            ca += temp_ca;
            cd += temp_cd;
            cs += temp_cs;
        }
    }
    d = glm::normalize(d);
    return directional;
}
