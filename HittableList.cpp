#include "HittableList.h"

HittableList::HittableList() { max_val = 1; }

HittableList::HittableList(std::vector<Hittable *> l) {
  list_ = l;
  max_val = 1;
}

inline bool HittableList::is_bounded() const {
  for (const Hittable* h : list_) {
    if (!h->is_bounded()) return false;
  }
  return true;
}

void HittableList::add_hittable(Hittable* h) {
  list_.push_back(h);
}

bool HittableList::hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                       Light &l) const {
  hit_record temp;
  bool hit = false;
  float closest = t_max;
  for (unsigned long i = 0; i < list_.size(); i++) {
    if (list_[i]->hit(r, t_min, closest, temp, l)) {
      hit = true;
      closest = temp.t;
      rec = temp;
    }
  }
  return hit;
}
