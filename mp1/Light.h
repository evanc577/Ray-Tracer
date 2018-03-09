#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include "glm/glm/glm.hpp"

class Light {
 public:
  Light() {}

  glm::vec3 ia = glm::vec3(1, 1, 1);
  glm::vec3 id = glm::vec3(1, 1, 1);
  glm::vec3 is = glm::vec3(1, 1, 1);

  // returns whether there is light at point p
  // sets ca, cd, and cs defining colors of ambient, diffuse, and specular
  // lights at point p.
  // sets d direction of light at point p
  virtual bool AtPoint(const glm::vec3 &p, std::vector<glm::vec3> &ca,
                       std::vector<glm::vec3> &cd, std::vector<glm::vec3> &cs,
                       std::vector<glm::vec3> &d) = 0;
};

#endif
