#include "LightList.h"
#include <iostream>

LightList::LightList() : Light() {}

LightList::LightList(std::vector<Light *> l) : Light() { list = l; }

void LightList::updateValues() {
    for (std::vector<Light *>::const_iterator it = list.begin();
         it != list.end(); ++it) {
        ia += (*it)->ia;
    }
}

bool LightList::AtPoint(const glm::vec3 &p, std::vector<glm::vec3> &ca,
                        std::vector<glm::vec3> &cd, std::vector<glm::vec3> &cs,
                        std::vector<glm::vec3> &d) {
    std::vector<glm::vec3> temp_ca;
    std::vector<glm::vec3> temp_cd;
    std::vector<glm::vec3> temp_cs;
    std::vector<glm::vec3> temp_d;

    bool directional = false;
    for (int i = 0; i < (int)list.size(); i++) {
        if (list[i]->AtPoint(p, temp_ca, temp_cd, temp_cs, temp_d))
            directional = true;

        ca.insert(ca.end(), temp_ca.begin(), temp_ca.end());
        cd.insert(cd.end(), temp_cd.begin(), temp_cd.end());
        cs.insert(cs.end(), temp_cs.begin(), temp_cs.end());

        for (int j = 0; j < (int)temp_d.size(); j++) {
            d.push_back(glm::normalize(temp_d[j]));
        }
    }
    return directional;
}
