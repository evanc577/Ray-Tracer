#ifndef AMBIENT_LIGHT_H
#define AMBIENT_LIGHT_H

#include "Light.h"

class AmbientLight : public Light {
 public:
  AmbientLight();

  virtual bool AtPoint(const glm::vec3 &p, std::vector<glm::vec3> &ca,
                       std::vector<glm::vec3> &cd, std::vector<glm::vec3> &cs,
                       std::vector<glm::vec3> &d);
};

#endif
