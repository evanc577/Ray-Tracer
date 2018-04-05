#ifndef AABB_H_
#define AABB_H_

#include <algorithm>
#include <stack>
#include <tuple>
#include "Hittable.h"
#include "Light.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float max(float a, float b) { return a > b ? a : b; }

class AABB {
 public:
  struct Node {
    Hittable *item;
    std::tuple<vec3, vec3> bounds;
    int offset;
  };

  AABB();
  AABB(const std::vector<Hittable*>& l);

  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const;


  std::vector<Node> tree_;
  std::vector<Hittable*> unbounded_list_;

 private:
  void generate_tree(const std::vector<Hittable*>& l);
  int partition(std::vector<Hittable*>& l, int left, int right, int pivot_index) const;
  void swap(Hittable*& a, Hittable*& b) const;
};

#endif
