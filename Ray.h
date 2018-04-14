#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
 public:
  Ray() {}
  Ray(const vec3 &a, const vec3 &b) {
    A_ = a;
    B_ = b;
    inv_B_ = vec3(1, 1, 1) / b;
  }
  inline vec3 origin() const { return A_; }
  // void setOrigin(const vec3 &o) { A_ = o; }
  inline vec3 direction() const { return B_; }
  inline vec3 inv_direction() const { return inv_B_; }
  // void setDirection(const vec3 &d) { B_ = d; }
  inline vec3 location(float t) const { return A_ + t * B_; }

 private:
  vec3 A_;
  vec3 B_;
  vec3 inv_B_;
};

#endif
