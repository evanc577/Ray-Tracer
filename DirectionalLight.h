#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight : public Light {
 public:
  DirectionalLight();

  vec3 direction;

  virtual bool AtPoint(const vec3 &p, std::vector<vec3> &ca,
                       std::vector<vec3> &cd, std::vector<vec3> &cs,
                       std::vector<vec3> &d);

  virtual float distance(const vec3& p) const;
};

#endif
