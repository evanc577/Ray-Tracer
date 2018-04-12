#include "PointLight.h"

PointLight::PointLight() : Light() { point = vec3(0, 0, 0); }

bool PointLight::AtPoint(const vec3 &p, std::vector<vec3> &ca,
                         std::vector<vec3> &cd, std::vector<vec3> &cs,
                         std::vector<vec3> &d) {
  ca.clear();
  cd.clear();
  cs.clear();
  d.clear();

  float distance = (p - point).length();

  ca.emplace_back(0, 0, 0);
  cd.push_back(id / (distance * distance));
  cs.push_back(is / (distance * distance));
  d.push_back(normalize(point - p));
  return true;
}

float PointLight::distance(const vec3 &p) const {
  return (p - point).length();
}
