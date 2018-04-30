#ifndef HITTABLE_H
#define HITTABLE_H

#include <cmath>
#include <limits>
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

  virtual bool hit_one(const Ray &r, float t_min, float t_max) const = 0;

  // returns a vector defining the color at the point defined in rec.
  // l is a light source, d is the direction from the camera to point
  virtual vec3 color(hit_record &rec, Light &l, const vec3 &d) {
    // calculate and return a list of ambient, directional, specular, and
    // light directions at rec.p
    std::vector<vec3> ca_list;
    std::vector<vec3> cd_list;
    std::vector<vec3> cs_list;
    std::vector<vec3> light_d_list;  // vector pointing towards light source
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

      // test for shadows
      hit_record shadow_rec;
      Ray shadow_ray(rec.p, light_d_list[i]);
      if (std::isnan(light_d_list[i][0]) | std::isnan(light_d_list[i][1]) |
          std::isnan(light_d_list[i][2])) {
        d_list.emplace_back(0, 0, 0);
        continue;
      }
      if (hit_one(shadow_ray, 0.0001f, l.distance(rec.p))) {
        d_list.emplace_back(0, 0, 0);
        continue;
      }

      // float dot_prod = dot(light_d_list[i], rec.normal);
      vec3 temp_diffuse;
      // if (dot_prod < 0.0001f && dot_prod > -0.0001f) {
      // temp_diffuse = vec3(0, 0, 0);
      // pos_diffuse_list[i] = false;
      // } else {
      temp_diffuse = cd_list[i] * rec.kd * dot(light_d_list[i], rec.normal);

      if (std::isnan(temp_diffuse[0]) || std::isnan(temp_diffuse[1]) ||
          std::isnan(temp_diffuse[2])) {
        temp_diffuse = vec3(0, 0, 0);
        pos_diffuse_list[i] = false;
      }

      pos_diffuse_list[i] = true;

      if (temp_diffuse[0] < 0) temp_diffuse[0] = 0;
      if (temp_diffuse[1] < 0) temp_diffuse[1] = 0;
      if (temp_diffuse[2] < 0) temp_diffuse[2] = 0;

      if (temp_diffuse[0] <= 0.0f && temp_diffuse[1] <= 0.0f &&
          temp_diffuse[2] <= 0.0f) {
        pos_diffuse_list[i] = false;
      }
      // }
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

  virtual ~Hittable() = default;

  vec3 ka = vec3(1, 1, 1);
  vec3 kd = vec3(1, 1, 1);
  vec3 ks = vec3(1, 1, 1);
  float alpha = 5;
  float max_val;

  inline virtual bool is_bounded() const = 0;

  virtual std::tuple<vec3, vec3> get_bounds() const {
    return std::make_tuple(vec3(0, 0, 0), vec3(0, 0, 0));
  }
  virtual vec3 get_center() const { return vec3(0, 0, 0); };
};
#endif
