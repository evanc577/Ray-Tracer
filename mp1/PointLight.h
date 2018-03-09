#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

class PointLight : public Light {
 public:
  PointLight() : Light(), point(glm::vec3(0, 0, 0)) {}

  glm::vec3 point;

  virtual bool AtPoint(const glm::vec3 &p, std::vector<glm::vec3> &ca,
                       std::vector<glm::vec3> &cd, std::vector<glm::vec3> &cs,
                       std::vector<glm::vec3> &d);
};

#endif
