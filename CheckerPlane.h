#ifndef CHECKER_PLANE_H
#define CHECKER_PLANE_H

#include <cmath>
#include "Plane.h"

class CheckerPlane : public Plane {
 public:
  CheckerPlane();
  CheckerPlane(vec3 p, vec3 n);

  inline bool is_bounded() const;
  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const;

  vec3 orthogonal(vec3 n) const;
  vec3 orthogonal(vec3 v1, vec3 v2) const;
  vec3 ka1;
  vec3 ka2;
  vec3 kd1;
  vec3 kd2;
  vec3 ks1;
  vec3 ks2;
  float alpha1;
  float alpha2;
  float tile_size;
};

#endif
