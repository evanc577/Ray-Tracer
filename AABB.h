#ifndef AABB_H_
#define AABB_H_

#include <algorithm>
#include <stack>
#include <tuple>
#include "Hittable.h"
#include "HittableList.h"
#include "Light.h"

class AABB : public Hittable {
 public:
  struct Node {
    Hittable *item;
    std::tuple<vec3, vec3> bounds;
    int offset;
  };

  AABB();

  ~AABB() {}

  inline bool is_bounded() const;

  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const;

  virtual bool hit_one(const Ray &r, float t_min, float t_max) const;

  virtual void add_hittable(Hittable* h);

  virtual void generate();
  
  vec3 get_center() const { return vec3(0, 0, 0); }

  std::tuple<vec3, vec3> get_bounds() const {
    return std::make_tuple(vec3(0,0,0), vec3(0,0,0));
  }

  std::vector<Node> tree_;
  std::vector<Hittable*> list_;
  std::vector<Hittable*> unbounded_list_;

 private:
  int partition(std::vector<Hittable*>& l, int left, int right, int pivot_index) const;
  void swap(Hittable*& a, Hittable*& b) const;
  inline bool intersects_BB(const Ray &r, int index) const;
};

#endif
