#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include "Hittable.h"
#include "Light.h"

class HittableList : public Hittable {
 public:
  HittableList();
  HittableList(std::vector<Hittable *> l);

  float max_val;

  inline bool is_bounded() const;

  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const;

  // returns a vector defining the color at the point defined in rec.
  // l is a light source, d is the direction from the camera to point
  virtual vec3 color(hit_record &rec, Light &l, const vec3 &d);

  std::vector<Hittable *> list_;
};

#endif
