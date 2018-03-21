#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include "vec3.h"

class Light {
 public:
  Light() {}

  vec3 ia = vec3(1, 1, 1);
  vec3 id = vec3(1, 1, 1);
  vec3 is = vec3(1, 1, 1);

  // returns whether there is light at point p
  // sets ca, cd, and cs defining colors of ambient, diffuse, and specular
  // lights at point p.
  // sets d direction of light at point p
  virtual bool AtPoint(const vec3 &p, std::vector<vec3> &ca,
                       std::vector<vec3> &cd, std::vector<vec3> &cs,
                       std::vector<vec3> &d) = 0;
};

#endif
