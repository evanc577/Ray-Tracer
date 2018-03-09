#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
  direction = glm::normalize(glm::vec3(0, 0, -1));
}

bool DirectionalLight::AtPoint(const glm::vec3 &p, std::vector<glm::vec3> &ca,
                               std::vector<glm::vec3> &cd,
                               std::vector<glm::vec3> &cs,
                               std::vector<glm::vec3> &d) {
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
