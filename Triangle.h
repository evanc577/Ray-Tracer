#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Hittable.h"

class Triangle : public Hittable {
 public:
  Triangle();
  Triangle(vec3 a, vec3 b, vec3 c);

  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const;

  virtual bool hit_one(const Ray &r, float t_min, float t_max) const;

  inline bool is_bounded() const;

  std::tuple<vec3, vec3> get_bounds() const;
  inline vec3 get_center() const;

  vec3 A;
  vec3 B;
  vec3 C;
};
#endif
