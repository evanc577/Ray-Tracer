#ifndef PLANE_H
#define PLANE_H

#include "Hittable.h"

class Plane : public Hittable {
 public:
  Plane();
  Plane(vec3 p, vec3 n);

  inline bool is_bounded() const;

  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const;

  vec3 point;
  vec3 normal;
};

#endif
