#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include "glm/glm/glm.hpp"

class Light {
    public:
        Light() {}

        glm::vec3 ia = glm::vec3(1,1,1);
        glm::vec3 id = glm::vec3(1,1,1);
        glm::vec3 is = glm::vec3(1,1,1);

        virtual bool AtPoint(const glm::vec3 &p, glm::vec3 &ca,
                glm::vec3 &cd, glm::vec3 &cs, glm::vec3 &d) = 0;
};

#endif
