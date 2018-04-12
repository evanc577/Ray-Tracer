#include "AmbientLight.h"

AmbientLight::AmbientLight() : Light() {}

bool AmbientLight::AtPoint(const vec3 &p, std::vector<vec3> &ca,
                           std::vector<vec3> &cd,
                           std::vector<vec3> &cs,
                           std::vector<vec3> &d) {
  (void)p;

  ca.clear();
  cd.clear();
  cs.clear();
  d.clear();

  ca.push_back(ia);
  cd.emplace_back(0, 0, 0);
  cs.emplace_back(0, 0, 0);
  d.emplace_back(0, 0, 0);
  return true;
}


float AmbientLight::distance(const vec3 &p) const {
  (void)p;
  return std::numeric_limits<float>::max();
}
