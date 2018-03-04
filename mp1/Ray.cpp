#include "Ray.h"

Ray::Ray() {}

Ray::Ray(const vec3 &a, const vec3 &b) : A_(a), B_(b) {}

vec3 Ray::origin() const { return A_; }

vec3 Ray::direction() const { return B_; }

vec3 Ray::location(float t) const { return A_ + t*B_; }
