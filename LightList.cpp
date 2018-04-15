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
  for (Light*& l : list) {
    if (l->AtPoint(p, temp_ca, temp_cd, temp_cs, temp_d))
      directional = true;

    ca.insert(ca.end(), temp_ca.begin(), temp_ca.end());
    cd.insert(cd.end(), temp_cd.begin(), temp_cd.end());
    cs.insert(cs.end(), temp_cs.begin(), temp_cs.end());

    for (vec3& v : temp_d) {
      if (isnan(v[0]) | isnan(v[1]) | isnan(v[2])) {
        std::cout << "NAN" << std::endl;
      }
      d.push_back(normalize(v));
    }
  }
  return directional;
}


float LightList::distance(const vec3 &p) const {
  float distance = std::numeric_limits<float>::max();
  for (const auto& l : list) {
    float new_distance = l->distance(p);
    if (new_distance < distance) {

      distance = new_distance;
    }
  }
  return distance;
}
