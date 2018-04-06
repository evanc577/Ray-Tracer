#include "AABB.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }
inline float ffmax(float a, float b, float c) { return ffmax(ffmax(a, b), c); }
inline float ffmin(float a, float b, float c) { return ffmin(ffmin(a, b), c); }


AABB::AABB() {}

void AABB::add_hittable(Hittable* h) {
  if (h->is_bounded()) {
    list_.push_back(h);
  } else {
    unbounded_list_.push_back(h);
  }
}

inline bool AABB::is_bounded() const { return unbounded_list_.empty(); }

void AABB::generate() {
  std::vector<Hittable*> temp_vector = list_;
  // for (auto h : temp_vector) {
    // std::cout << std::endl
              // << "<" << h->get_center()[0] << "," << h->get_center()[1] << ","
              // << h->get_center()[2] << ">";
  // }
  // std::cout << std::endl;

  // tuples in form (left_index, right_index, axis)
  std::stack<std::tuple<int, int, int>> stk;

  stk.push(std::make_tuple(0, temp_vector.size() - 1, 0));
  while (!stk.empty()) {
    int left = std::get<0>(stk.top());
    int right = std::get<1>(stk.top());
    int d = std::get<2>(stk.top());
    stk.pop();

    // sort section of temp vector with respect to dimension d
    std::sort(temp_vector.begin() + left, temp_vector.begin() + right,
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

  // for (Node n : tree_) {
    // std::cout << "(";
    // std::cout << std::get<0>(n.bounds)[0] << ",";
    // std::cout << std::get<0>(n.bounds)[1] << ",";
    // std::cout << std::get<0>(n.bounds)[2] << ") | (";
    // std::cout << std::get<1>(n.bounds)[0] << ",";
    // std::cout << std::get<1>(n.bounds)[1] << ",";
    // std::cout << std::get<1>(n.bounds)[2] << ")";
    // std::cout << "  offset=" << n.offset;
    // if (n.offset == 0) {
      // std::cout << "  LEAF";
    // }
    // std::cout << std::endl;
  // }
}

bool AABB::intersects_BB(const Ray& r, int index, int t_min, int t_max) const {
  const std::tuple<vec3, vec3>& bounds = tree_[index].bounds;
  const vec3& lower = std::get<0>(bounds);
  const vec3& upper = std::get<1>(bounds);


  float tx1 = (lower[0] - r.origin()[0])/r.direction()[0];
  float tx2 = (upper[0] - r.origin()[0])/r.direction()[0];
  float ty1 = (lower[1] - r.origin()[1])/r.direction()[1];
  float ty2 = (upper[1] - r.origin()[1])/r.direction()[1];
  float tz1 = (lower[2] - r.origin()[2])/r.direction()[2];
  float tz2 = (upper[2] - r.origin()[2])/r.direction()[2];

  t_min = ffmax(ffmin(tx1, tx2), ffmin(ty1, ty2), ffmin(tz1, tz2));
  t_max = ffmin(ffmax(tx1, tx2), ffmax(ty1, ty2), ffmax(tz1, tz2));

  if (t_max < 0) {
    return false;
  }

  if (t_min > t_max) {
    return false;
  }

  return true;


  // t_min = ffmin(tx1, tx2);
  // t_max = ffmax(tx1, tx2);


  // t_min = ffmax(t_min, ffmin(ty1, ty2));
  // t_max = ffmax(t_max, ffmax(ty1, ty2));
  

  // t_min = ffmax(t_min, ffmin(tz1, tz2));
  // t_max = ffmax(t_max, ffmax(tz1, tz2));

  // return t_max >= t_min;
}

bool AABB::hit(const Ray& r, float t_min, float t_max, hit_record& rec,
               Light& l) const {
  hit_record temp;
  bool hit = false;
  float closest = t_max;
  std::stack<int> stk;
  stk.push(0);
  while (!stk.empty()) {
    int i = stk.top();
    stk.pop();
    if (intersects_BB(r, i, t_min, t_max)) {
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
  for (unsigned long i = 0; i < unbounded_list_.size(); i++) {
    if (unbounded_list_[i]->hit(r, t_min, closest, temp, l)) {
      hit = true;
      closest = temp.t;
      rec = temp;
    }
  }
  return hit;
}

vec3 AABB::color(hit_record &rec, Light &l, const vec3 &d) {
  // calculate and return a list of ambient, directional, specular, and
  // light directions at rec.p
  std::vector<vec3> ca_list;
  std::vector<vec3> cd_list;
  std::vector<vec3> cs_list;
  std::vector<vec3> light_d_list;
  l.AtPoint(rec.p, ca_list, cd_list, cs_list, light_d_list);

  // Phong model values
  std::vector<vec3> r_list;  // R vector
  std::vector<vec3> a_list;  // ambient term
  std::vector<vec3> d_list;  // diffuse term
  std::vector<vec3> s_list;  // specular term

  int size = ca_list.size();
  r_list.reserve(size);
  a_list.reserve(size);
  d_list.reserve(size);
  s_list.reserve(size);

  for (int i = 0; i < size; i++) {
    // add R vectors to r_list
    r_list.push_back(light_d_list[i] -
                     2 * (dot(light_d_list[i], rec.normal)) * rec.normal);

    // add ambient term to a_list
    a_list.push_back(ca_list[i] * rec.ka);
  }

  // determines if specular term should be added
  std::vector<bool> pos_diffuse_list;

  // add diffuse term to d_list;
  for (int i = 0; i < size; i++) {
    pos_diffuse_list.emplace_back(false);
    float dot_prod = dot(light_d_list[i], rec.normal);
    vec3 temp_diffuse;
    if (dot_prod < 0.0001f) {
      temp_diffuse = vec3(0, 0, 0);
      pos_diffuse_list[i] = false;
    } else {
      temp_diffuse = cd_list[i] * rec.kd * dot(light_d_list[i], rec.normal);

      if (isnan(temp_diffuse[0]) || isnan(temp_diffuse[1]) ||
          isnan(temp_diffuse[2])) {
        temp_diffuse = vec3(0, 0, 0);
        pos_diffuse_list[i] = false;
      }

      pos_diffuse_list[i] = true;

      if (temp_diffuse[0] < 0) temp_diffuse[0] = 0;
      if (temp_diffuse[1] < 0) temp_diffuse[1] = 0;
      if (temp_diffuse[2] < 0) temp_diffuse[2] = 0;

      if (temp_diffuse[0] <= 0.0001f && temp_diffuse[1] <= 0.0001f &&
          temp_diffuse[2] <= 0.0001f) {
        pos_diffuse_list[i] = false;
      }
    }
    d_list.push_back(temp_diffuse);
  }

  // add specular term to s_list
  for (int i = 0; i < size; i++) {
    vec3 temp_d = -d;
    if (pos_diffuse_list[i]) {
      vec3 specular(0, 0, 0);
      specular = rec.ks *
                 float((pow(-1.0f * dot(r_list[i], temp_d), rec.alpha))) *
                 cs_list[i];
      if (isnan(specular[0]) || isnan(specular[1]) ||
          isnan(specular[2])) {
        specular = vec3(0, 0, 0);
      }
      if (specular[0] < 0) specular[0] = 0;
      if (specular[1] < 0) specular[1] = 0;
      if (specular[2] < 0) specular[2] = 0;

      s_list.push_back(specular);
    } else {
      s_list.emplace_back(0, 0, 0);
    }
  }

  vec3 temp(0, 0, 0);
  for (int i = 0; i < size; i++) {
    temp += a_list[i];
  }
  for (int i = 0; i < size; i++) {
    temp += d_list[i];
  }
  for (int i = 0; i < size; i++) {
    temp += s_list[i];
  }
  if (temp[0] > max_val) max_val = temp[0];
  if (temp[1] > max_val) max_val = temp[1];
  if (temp[2] > max_val) max_val = temp[2];

  return temp;
}
