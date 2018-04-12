#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

class PointLight : public Light {
 public:
  PointLight();

  vec3 point;

  virtual bool AtPoint(const vec3 &p, std::vector<vec3> &ca,
                       std::vector<vec3> &cd, std::vector<vec3> &cs,
                       std::vector<vec3> &d);

  virtual float distance(const vec3 &p) const;
};

#endif
