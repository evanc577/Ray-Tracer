#include "Plane.h"

Plane::Plane() : point(vec3(0, 0, 0)), normal(vec3(0, 0, 1)) {}
Plane::Plane(vec3 p, vec3 n) : point(p), normal(normalize(n)) {}

bool Plane::hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                Light &l) const {
  (void)l;
  float temp = dot(normal, r.direction());
  if (temp > 0.0001f || temp < -0.0001f) {
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
  }
  return false;
}
