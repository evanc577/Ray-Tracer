#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
    direction = unit_vector(vec3(0,0,-1));
}

bool DirectionalLight::AtPoint(const vec3 &p, vec3 &c, vec3 &d) {
    (void)p;
    c = color + ia;
    d = -1.0*direction;
    return true;
}
