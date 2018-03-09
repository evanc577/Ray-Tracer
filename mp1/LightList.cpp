#include "LightList.h"
#include <iostream>

LightList::LightList() : Light() {}

LightList::LightList(std::vector<Light *> l) : Light() {list = l;}

void LightList::updateValues() {
    for (std::vector<Light *>::const_iterator it = list.begin();
            it != list.end(); ++it) {
        ia += (*it)->ia;
    }
}

bool LightList::AtPoint(const glm::vec3 &p, std::vector<glm::vec3> &ca,
        std::vector<glm::vec3> &cd, std::vector<glm::vec3> &cs,
        std::vector<glm::vec3> &d) {

    ca.clear();
    cd.clear();
    cs.clear();
    d.clear();

    std::vector<glm::vec3> temp_ca;
    std::vector<glm::vec3> temp_cd;
    std::vector<glm::vec3> temp_cs;
    std::vector<glm::vec3> temp_d;


    bool directional = false;
    for (int i = 0; i < (int)list.size(); i++) {
        if (list[i]->AtPoint(p, temp_ca, temp_cd, temp_cs, temp_d)) {
            directional = true;
        }

        for (int j = 0; j < (int)temp_ca.size(); j++) {
            ca.push_back(temp_ca[j]);
        }
        for (int j = 0; j < (int)temp_cd.size(); j++) {
            cd.push_back(temp_cd[j]);
            // std::cout << cd[j][0] << " " << cd[j][1] << " " << cd[j][2] << "\n";
        }
        for (int j = 0; j < (int)temp_cs.size(); j++) {
            cs.push_back(temp_cs[j]);
        }

        for (int j = 0; j < (int)temp_d.size(); j++) {
            temp_d[j] = glm::normalize(temp_d[j]);
            d.push_back(temp_d[j]);
            // std::cout << d[j][0] << " " << d[j][1] << " " << d[j][2] << "\n";
        }
        
    }
    return directional;
}
