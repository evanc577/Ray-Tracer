#include "LightList.h"

LightList::LightList() : Light() {
    ia = glm::vec3(0,0,0);
}

LightList::LightList(std::vector<Light *> l) : Light() {
    ia = glm::vec3(0,0,0);
    list = l;
}

void LightList::updateValues() {
    for (std::vector<Light *>::const_iterator it = list.begin();
            it != list.end(); ++it) {
        ia += (*it)->ia;
    }
}

bool LightList::AtPoint(const glm::vec3 &p, glm::vec3 &c, glm::vec3 &d) {
    c = glm::vec3(0,0,0);
    d = glm::vec3(0,0,0);
    glm::vec3 temp_c(0,0,0);
    glm::vec3 temp_d(0,0,0);
    bool directional = false;
    for (std::vector<Light *>::const_iterator it = list.begin();
            it != list.end(); ++it) {
        if ((*it)->AtPoint(p, temp_c, temp_d)) {
            directional = true;
            d += temp_d;
            c += temp_c;
        }
    }
    d = glm::normalize(d);
    return directional;
}
