#ifndef AABB_H_
#define AABB_H_

#include <tuple>
#include "Hittable.h"
#include "HittableList.h"
#include "Light.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float max(float a, float b) { return a > b ? a : b; }

class AABB : HittableList {
 public:
  struct Node {
    Hittable *item;
    std::tuple<vec3, vec3> bounds;
    int right_offset;
  };

  AABB();
  AABB(std::vector<Hittable *> l);

  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const;

  void generate_tree();

  std::vector<Node> tree_;
  std::vector<Hittable*> unbounded_list_;
};

#endif
