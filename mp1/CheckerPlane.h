#ifndef CHECKER_PLANE_H
#define CHECKER_PLANE_H

#include <cmath>
#include "Plane.h"

class CheckerPlane : public Plane {
 public:
  CheckerPlane();
  CheckerPlane(glm::vec3 p, glm::vec3 n);

  virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec,
                   Light &l) const;

  glm::vec3 orthogonal(glm::vec3 n) const;
  glm::vec3 orthogonal(glm::vec3 v1, glm::vec3 v2) const;
  glm::vec3 ka1;
  glm::vec3 ka2;
  glm::vec3 kd1;
  glm::vec3 kd2;
  glm::vec3 ks1;
  glm::vec3 ks2;
  float alpha1;
  float alpha2;
  float tile_size;
};

#endif
