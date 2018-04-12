#ifndef LIGHT_LIST_H
#define LIGHT_LIST_H

#include <vector>
#include "Light.h"

class LightList : public Light {
 public:
  LightList();
  LightList(std::vector<Light *> l);

  virtual bool AtPoint(const vec3 &p, std::vector<vec3> &ca,
                       std::vector<vec3> &cd, std::vector<vec3> &cs,
                       std::vector<vec3> &d);

  virtual float distance(const vec3 &p) const;

  std::vector<Light *> list;
};

#endif
