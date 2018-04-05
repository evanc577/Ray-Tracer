#include "AABB.h"

AABB::AABB() {}

AABB::AABB(const std::vector<Hittable*>& l) { generate_tree(l); }

void AABB::generate_tree(const std::vector<Hittable*>& l) {
  std::vector<Hittable*> temp_vector = l;
  // tuples in form (left_index, right_index, axis)
  std::stack<std::tuple<int, int, int>> stk;

  stk.push(std::make_tuple(0, temp_vector.size(), 0));
  while (!stk.empty()) {
    int left = std::get<0>(stk.top());
    int right = std::get<1>(stk.top());
    int d = std::get<2>(stk.top());
    stk.pop();

    std::sort(temp_vector.begin() + left, temp_vector.end() + right,
              [&](Hittable* a, Hittable* b) {
                return a->get_center()[d] < b->get_center()[d];
              });

    // calculate bounding box
    vec3 lower, upper;
    for (auto it = temp_vector.begin() + left;
         it != temp_vector.begin() + right; ++it) {
      std::tuple<vec3, vec3> b = (*it)->get_bounds();
      if (std::get<0>(b)[0] < lower[0]) lower[0] = std::get<0>(b)[0];
      if (std::get<0>(b)[1] < lower[1]) lower[1] = std::get<0>(b)[1];
      if (std::get<0>(b)[2] < lower[2]) lower[1] = std::get<0>(b)[2];
      if (std::get<0>(b)[0] > upper[0]) upper[0] = std::get<0>(b)[0];
      if (std::get<0>(b)[1] > upper[1]) upper[1] = std::get<0>(b)[1];
      if (std::get<0>(b)[2] > upper[2]) upper[1] = std::get<0>(b)[2];
    }

    // create BVH nodes
    int pivot = (right - left) / 2;
    Node n = {nullptr, std::make_tuple(lower, upper), pivot / 2 - left - 1};
    if (right == left) {
      n.item = temp_vector[right];
      n.offset = 0;
    }
    tree_.push_back(n);

    // add left & right halves to stack
    if (pivot < right) {
      stk.push(std::make_tuple(pivot + 1, right, (d + 1) % 3));
    }
    if (pivot > left) {
      stk.push(std::make_tuple(left, pivot - 1, (d + 1) % 3));
    }
  }
}
