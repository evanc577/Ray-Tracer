#ifndef LIGHT_H
#define LIGHT_H

#include <vector>
#include "glm/glm/glm.hpp"

class Light {
    public:
        Light() : color(glm::vec3(1,1,1)), ia(glm::vec3(0,0,0)) {}

        glm::vec3 color;

        glm::vec3 ia;

        virtual bool AtPoint(const glm::vec3 &p, glm::vec3 &c,
                glm::vec3 &d) = 0;
};

#endif
