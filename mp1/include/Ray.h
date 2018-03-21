#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
 public:
  Ray() {}
  Ray(const vec3 &a, const vec3 &b) : A_(a), B_(b) {}
  vec3 origin() const { return A_; }
  void setOrigin(const vec3 &o) { A_ = o; }
  vec3 direction() const { return B_; }
  void setDirection(const vec3 &d) { B_ = d; }
  vec3 location(float t) const { return A_ + t * B_; }

 private:
  vec3 A_;
  vec3 B_;
};

#endif
