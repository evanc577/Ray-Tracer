#include "Triangle.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }
inline float ffmax(float a, float b, float c) { return ffmax(ffmax(a, b), c); }
inline float ffmin(float a, float b, float c) { return ffmin(ffmin(a, b), c); }

Triangle::Triangle() : A(vec3(0, 0, 0)), B(vec3(0, 0, 0)), C(vec3(0, 0, 0)) {}

Triangle::Triangle(vec3 a, vec3 b, vec3 c) : A(a), B(b), C(c) {}

inline bool Triangle::is_bounded() const { return true; }

std::tuple<vec3, vec3> Triangle::get_bounds() const {
  vec3 a(ffmin(A[0], B[0], C[0]), ffmin(A[1], B[1], C[1]),
         ffmin(A[2], B[2], C[2]));
  vec3 b(ffmax(A[0], B[0], C[0]), ffmax(A[1], B[1], C[1]),
         ffmax(A[2], B[2], C[2]));
  return std::make_tuple(a, b);
}

inline vec3 Triangle::get_center() const {
  return vec3((A[0] + B[0] + C[0]) / 3, (A[1] + B[1] + C[1]) / 3,
              (A[2] + B[2] + C[2]) / 3);
}

bool Triangle::hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const {
  (void)l;

  vec3 edge1, edge2, n, h, s, q;
  float a, f, u, v;
  edge1 = B - A;
  edge2 = C - A;
  n = cross(edge1, edge2);  // vector normal to plane of triangle

  h = cross(r.direction(), edge2);
  a = dot(edge1, h);
  if (a < 0.0001f && a > -0.0001f) return false;

  f = 1 / a;
  s = r.origin() - A;
  u = f * dot(s, h);
  if (u < 0.0f || u > 1.0f) return false;
  q = cross(s, edge1);
  v = f * dot(r.direction(), q);
  if (v < 0.0f || u + v > 1.0f) return false;

  float t = f * dot(edge2, q);

  if (t < t_max && t > t_min) {
    rec.t = t;
    rec.p = r.location(rec.t);
    rec.normal = normalize(n);
    if (dot(r.direction(), n) > 0) {
      rec.normal *= -1;
    }
    rec.ka = ka;
    rec.kd = kd;
    rec.ks = ks;
    rec.alpha = alpha;
    return true;
  }
  return false;
}

bool Triangle::hit_one(const Ray &r, float t_min, float t_max) const {
  vec3 edge1, edge2, n, h, s, q;
  float a, f, u, v;
  edge1 = B - A;
  edge2 = C - A;
  n = cross(edge1, edge2);  // vector normal to plane of triangle

  h = cross(r.direction(), edge2);
  a = dot(edge1, h);
  if (a < 0.0001f && a > -0.0001f) return false;

  f = 1 / a;
  s = r.origin() - A;
  u = f * dot(s, h);
  if (u < 0.0f || u > 1.0f) return false;
  q = cross(s, edge1);
  v = f * dot(r.direction(), q);
  if (v < 0.0f || u + v > 1.0f) return false;

  float t = f * dot(edge2, q);

  if (t < t_max && t > t_min) {
    return true;
  }
  return false;
}
