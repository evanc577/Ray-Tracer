#include "Triangle.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }
inline float ffmax(float a, float b, float c) { return ffmax(ffmax(a, b), c); }
inline float ffmin(float a, float b, float c) { return ffmin(ffmin(a, b), c); }

Triangle::Triangle() {
  A = vec3(0,0,0);
  B = vec3(0,0,0);
  C = vec3(0,0,0);
  ia = 0;
  ib = 0;
  ic = 0;
  smooth_shading = false;
}

Triangle::Triangle(vec3 a, vec3 b, vec3 c) : Triangle() {
  A = a;
  B = b;
  C = c;
}

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

  vec3 AB = B - A;
  vec3 AC = C - A;
  vec3 pvec = cross(r.direction(), AC);
  float det = dot(AB, pvec);

  float inv_det = 1 / det;
  vec3 tvec = r.origin() - A;
  float u = dot(tvec, pvec) * inv_det;
  if (u < 0 || u > 1) {
    return false;
  }

  vec3 qvec = cross(tvec, AB);
  float v = dot(r.direction(), qvec) * inv_det;
  if (v < 0 || u + v > 1) {
    return false;
  }

  float t = dot(AC, qvec) * inv_det;



  if (t < t_max && t > t_min) {
    rec.t = t;
    rec.p = r.location(rec.t);
    vec3 norm;
    if (smooth_shading) {
      norm = u * normB + v * normC + (1 - u - v) * normA;
    } else {
      norm = cross(AB, AC);
    }
    rec.normal = normalize(norm);
    if (dot(r.direction(), rec.normal) > 0) {
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
  vec3 AB = B - A;
  vec3 AC = C - A;
  vec3 pvec = cross(r.direction(), AC);
  float det = dot(AB, pvec);

  float inv_det = 1 / det;
  vec3 tvec = r.origin() - A;
  float u = dot(tvec, pvec) * inv_det;
  if (u < 0 || u > 1) {
    return false;
  }

  vec3 qvec = cross(tvec, AB);
  float v = dot(r.direction(), qvec) * inv_det;
  if (v < 0 || u + v > 1) {
    return false;
  }

  float t = dot(AC, qvec) * inv_det;

  if (t < t_max && t > t_min) {
    return true;
  }
  return false;
}
