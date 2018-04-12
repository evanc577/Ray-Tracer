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

  virtual bool hit_one(const Ray &r, float t_min, float t_max) const;

  std::tuple<vec3, vec3> get_bounds() const {
    return std::make_tuple(vec3(0,0,0), vec3(0,0,0));
  }

  vec3 get_center() const { return vec3(0,0,0); }

  vec3 point;
  vec3 normal;
};

#endif
