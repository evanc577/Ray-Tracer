#include "Sphere.h"

Sphere::Sphere() : center(vec3(0, 0, 0)), radius(1) {}

Sphere::Sphere(vec3 c, float r) : center(c), radius(r) {}

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec,
                 Light& l) const {
  (void)l;
  vec3 oc = r.origin() - center;
  float a = dot(r.direction(), r.direction());
  float b = dot(oc, r.direction());
  float c = dot(oc, oc) - radius * radius;
  float discriminant = b * b - a * c;
  if (discriminant > 0) {
    float temp = (-b - sqrt(discriminant)) / (a);
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.location(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.ka = ka;
      rec.kd = kd;
      rec.ks = ks;
      rec.alpha = alpha;
      return true;
    }
    temp = (-b + sqrt(discriminant)) / (a);
    if (temp < t_max && temp > t_min) {
      rec.t = temp;
      rec.p = r.location(rec.t);
      rec.normal = (rec.p - center) / radius;
      rec.ka = ka;
      rec.kd = kd;
      rec.ks = ks;
      rec.alpha = alpha;
      rec.normal *= -1;
      return true;
    }
  }
  return false;
}
