#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include "Hittable.h"
#include "Light.h"

class HittableList : public Hittable {
 public:
  HittableList();
  HittableList(std::vector<Hittable *> l);

  inline bool is_bounded() const;

  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const;

  virtual void add_hittable(Hittable* h);

  virtual void generate() {}

  std::vector<Hittable *> list_;

  std::tuple<vec3, vec3> get_bounds() const {
    return std::make_tuple(vec3(0,0,0), vec3(0,0,0));
  }
  vec3 get_center() const { return vec3(0,0,0); }
};

#endif
