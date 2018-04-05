#ifndef HITTABLE_H
#define HITTABLE_H

#include <tuple>
#include "Light.h"
#include "Ray.h"

struct hit_record {
  float t;
  vec3 p;
  vec3 normal;

  vec3 ka;

  vec3 kd;
  vec3 l;

  vec3 ks;
  vec3 r;
  float alpha;
};

class Hittable {
 public:
  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const = 0;

  virtual ~Hittable() {}

  vec3 ka = vec3(1, 1, 1);
  vec3 kd = vec3(1, 1, 1);
  vec3 ks = vec3(1, 1, 1);
  float alpha = 5;

  inline virtual bool is_bounded() const = 0;

  std::tuple<vec3, vec3> get_bounds() const;
};
#endif
