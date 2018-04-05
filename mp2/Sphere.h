#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"

class Sphere : public Hittable {
 public:
  Sphere();
  Sphere(vec3 c, float r);
  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const;

  inline virtual bool is_bounded() const;
  virtual std::tuple<vec3, vec3> get_bounds() const;

  vec3 center;
  float radius;

};

#endif
