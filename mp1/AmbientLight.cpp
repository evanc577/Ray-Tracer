#include "AmbientLight.h"

AmbientLight::AmbientLight() : Light() {}

bool AmbientLight::AtPoint(const vec3 &p, vec3 &c, vec3 &d) {
    (void)p;
    (void)d;
    c = ia;
    return false;
}
