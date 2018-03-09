#ifndef LIGHT_LIST_H
#define LIGHT_LIST_H

#include <vector>
#include "Light.h"

class LightList : public Light {
 public:
  LightList();
  LightList(std::vector<Light *> l);

  virtual bool AtPoint(const glm::vec3 &p, std::vector<glm::vec3> &ca,
                       std::vector<glm::vec3> &cd, std::vector<glm::vec3> &cs,
                       std::vector<glm::vec3> &d);

  std::vector<Light *> list;
};

#endif
