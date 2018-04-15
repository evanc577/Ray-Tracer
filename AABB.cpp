#include "AABB.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

AABB::AABB() { max_val = 1; }

void AABB::add_hittable(Hittable* h) {
  if (h->is_bounded()) {
    list_.push_back(h);
  } else {
    unbounded_list_.push_back(h);
  }
}

inline bool AABB::is_bounded() const { return unbounded_list_.empty(); }

void AABB::generate() {
  if (list_.empty()) {
    return;
  }
  std::vector<Hittable*> temp_vector = list_;

  // tuples in form (left_index, right_index, axis)
  std::stack<std::tuple<int, int, int>> stk;

  stk.push(std::make_tuple(0, temp_vector.size() - 1, 0));
  while (!stk.empty()) {
    int left = std::get<0>(stk.top());
    int right = std::get<1>(stk.top());
    int d = std::get<2>(stk.top());
    stk.pop();

    // partition section of temp vector with respect to dimension d around
    // median
    std::nth_element(temp_vector.begin() + left,
                     temp_vector.begin() + (right - left) / 2 + left,
                     temp_vector.begin() + right,
                     [&](Hittable* a, Hittable* b) {
                       return a->get_center()[d] < b->get_center()[d];
                     });

    // calculate bounding box
    std::tuple<vec3, vec3> b = temp_vector[left]->get_bounds();
    vec3 lower = std::get<0>(b);
    vec3 upper = std::get<1>(b);
    // std::cout << "upper: " << upper[0] << std::endl;
    int pivot = (right - left) / 2 + left;

    // directly push to tree_ if we have 1 or 2 nodes
    if (left == right) {
      Node n = {temp_vector[left], b, 0};
      tree_.push_back(n);
      continue;
    }

    // calculate max bounding box for section of vector
    for (auto it = temp_vector.begin() + left + 1;
         it != temp_vector.begin() + right + 1; ++it) {
      b = (*it)->get_bounds();  // bounds of object
      vec3 temp_lower = std::get<0>(b);
      vec3 temp_upper = std::get<1>(b);
      if (temp_lower[0] < lower[0]) lower[0] = temp_lower[0];
      if (temp_lower[1] < lower[1]) lower[1] = temp_lower[1];
      if (temp_lower[2] < lower[2]) lower[2] = temp_lower[2];

      if (temp_upper[0] > upper[0]) upper[0] = temp_upper[0];
      if (temp_upper[1] > upper[1]) upper[1] = temp_upper[1];
      if (temp_upper[2] > upper[2]) upper[2] = temp_upper[2];
    }

    // // create BVH nodes
    Node n = {nullptr, std::make_tuple(lower, upper), 2 * (pivot - left + 1)};
    if (left + 1 == right) n.offset = 2;
    tree_.push_back(n);

    if (left + 1 == right) {
      b = temp_vector[left]->get_bounds();
      Node n_left = {temp_vector[left], b, 0};
      b = temp_vector[right]->get_bounds();
      Node n_right = {temp_vector[right], b, 0};
      tree_.push_back(n_left);
      tree_.push_back(n_right);
      continue;
    }

    // add left & right halves to stack
    if (pivot < right) {
      stk.push(std::make_tuple(pivot + 1, right, (d + 1) % 3));
    }
    if (pivot > left) {
      stk.push(std::make_tuple(left, pivot, (d + 1) % 3));
    }
  }
}

bool AABB::intersects_BB(const Ray& r, int index) const {
  const std::tuple<vec3, vec3>& bounds = tree_[index].bounds;
  const vec3& lower = std::get<0>(bounds);
  const vec3& upper = std::get<1>(bounds);

  float t1 = (lower[0] - r.origin()[0]) * r.inv_direction()[0];
  float t2 = (upper[0] - r.origin()[0]) * r.inv_direction()[0];

  float tmin = ffmin(t1, t2);
  float tmax = ffmax(t1, t2);

  for (int i = 0; i < 3; ++i) {
    t1 = (lower[i] - r.origin()[i]) * r.inv_direction()[i];
    t2 = (upper[i] - r.origin()[i]) * r.inv_direction()[i];

    tmin = ffmax(tmin, ffmin(t1, t2));
    tmax = ffmin(tmax, ffmax(t1, t2));
  }

  return tmax >= ffmax(tmin, 0.0);
}

bool AABB::hit(const Ray& r, float t_min, float t_max, hit_record& rec,
               Light& l) const {
  hit_record temp;
  bool hit = false;
  float closest = t_max;
  std::stack<int> stk;
  stk.push(0);
  while (!tree_.empty() && !stk.empty()) {
    int i = stk.top();
    stk.pop();
    if (intersects_BB(r, i)) {
      if (tree_[i].offset == 0) {
        if (tree_[i].item->hit(r, t_min, closest, temp, l)) {
          hit = true;
          closest = temp.t;
          rec = temp;
        }
      } else {
        stk.push(i + tree_[i].offset);
        stk.push(i + 1);
      }
    }
  }
  for (auto& h : unbounded_list_) {
    if (h->hit(r, t_min, closest, temp, l)) {
      hit = true;
      closest = temp.t;
      rec = temp;
    }
  }
  return hit;
}

bool AABB::hit_one(const Ray& r, float t_min, float t_max) const {
  float closest = t_max;
  std::stack<int> stk;
  stk.push(0);
  while (!tree_.empty() && !stk.empty()) {
    int i = stk.top();
    stk.pop();
    if (intersects_BB(r, i)) {
      if (tree_[i].offset == 0) {
        if (tree_[i].item->hit_one(r, t_min, closest)) return true;
      } else {
        stk.push(i + tree_[i].offset);
        stk.push(i + 1);
      }
    }
  }
  for (unsigned long i = 0; i < unbounded_list_.size(); ++i) {
    if (unbounded_list_[i]->hit_one(r, t_min, closest)) {
      return true;
    }
  }
  return false;
}
