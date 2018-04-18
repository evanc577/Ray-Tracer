#include "Plane.h"

Plane::Plane() : point(vec3(0, 0, 0)), normal(vec3(0, 0, 1)) {}
Plane::Plane(vec3 p, vec3 n) : point(p), normal(normalize(n)) {}

inline bool Plane::is_bounded() const { return true; }

bool Plane::hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                Light &l) const {
  (void)l;
  float temp = dot(normal, r.direction());
  float t = dot((point - r.origin()), normal) / temp;
  if (t < t_max && t > t_min) {
    rec.t = t;
    rec.p = r.location(rec.t);
    rec.normal = normal;
    rec.ka = ka;
    rec.kd = kd;
    rec.ks = ks;
    rec.alpha = alpha;
    return true;
  }
  return false;
}

bool Plane::hit_one(const Ray &r, float t_min, float t_max) const {
  float temp = dot(normal, r.direction());
  float t = dot((point - r.origin()), normal) / temp;
  if (t < t_max && t > t_min) {
    return true;
  }
  return false;
}
