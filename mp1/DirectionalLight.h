#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight : public Light {
 public:
  DirectionalLight();

  glm::vec3 direction;

  virtual bool AtPoint(const glm::vec3 &p, std::vector<glm::vec3> &ca,
                       std::vector<glm::vec3> &cd, std::vector<glm::vec3> &cs,
                       std::vector<glm::vec3> &d);
};

#endif
