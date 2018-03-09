#ifndef RAY_H
#define RAY_H

#include "glm/glm/glm.hpp"

class Ray {
 public:
  Ray() {}
  Ray(const glm::vec3 &a, const glm::vec3 &b) : A_(a), B_(b) {}
  glm::vec3 origin() const { return A_; }
  void setOrigin(const glm::vec3 &o) { A_ = o; }
  glm::vec3 direction() const { return B_; }
  void setDirection(const glm::vec3 &d) { B_ = d; }
  glm::vec3 location(float t) const { return A_ + t * B_; }

 private:
  glm::vec3 A_;
  glm::vec3 B_;
};

#endif
