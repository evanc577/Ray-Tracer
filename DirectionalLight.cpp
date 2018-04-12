#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
  direction = normalize(vec3(0, 0, -1));
}

bool DirectionalLight::AtPoint(const vec3 &p, std::vector<vec3> &ca,
                               std::vector<vec3> &cd,
                               std::vector<vec3> &cs,
                               std::vector<vec3> &d) {
  (void)p;
  ca.clear();
  cd.clear();
  cs.clear();
  d.clear();

  ca.emplace_back(0, 0, 0);
  cd.push_back(id);
  cs.push_back(is);
  d.push_back(-1.0f * direction);
  return true;
}

float DirectionalLight::distance(const vec3& p) const {
  (void)p;
  return std::numeric_limits<float>::max();
}
