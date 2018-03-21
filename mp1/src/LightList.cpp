#include "LightList.h"

LightList::LightList() : Light() {}

LightList::LightList(std::vector<Light *> l) : Light() { list = l; }

bool LightList::AtPoint(const vec3 &p, std::vector<vec3> &ca,
                        std::vector<vec3> &cd, std::vector<vec3> &cs,
                        std::vector<vec3> &d) {
  std::vector<vec3> temp_ca;
  std::vector<vec3> temp_cd;
  std::vector<vec3> temp_cs;
  std::vector<vec3> temp_d;

  bool directional = false;
  for (int i = 0; i < (int)list.size(); i++) {
    if (list[i]->AtPoint(p, temp_ca, temp_cd, temp_cs, temp_d))
      directional = true;

    ca.insert(ca.end(), temp_ca.begin(), temp_ca.end());
    cd.insert(cd.end(), temp_cd.begin(), temp_cd.end());
    cs.insert(cs.end(), temp_cs.begin(), temp_cs.end());

    for (int j = 0; j < (int)temp_d.size(); j++) {
      d.push_back(normalize(temp_d[j]));
    }
  }
  return directional;
}
