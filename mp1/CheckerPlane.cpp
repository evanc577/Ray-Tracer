#include "CheckerPlane.h"

CheckerPlane::CheckerPlane() : Plane() {
    ka1 = glm::vec3(1,1,1);
    ka2 = glm::vec3(0,0,0);
    kd1 = ka1;
    kd2 = ka2;
    ks1 = ka1;
    ks2 = ka2;
    alpha1 = 5;
    alpha2 = alpha1;
}
CheckerPlane::CheckerPlane(glm::vec3 p, glm::vec3 n) : Plane(p,n) {
    ka1 = glm::vec3(1,1,1);
    ka2 = glm::vec3(0,0,0);
    kd1 = ka1;
    kd2 = ka2;
    ks1 = ka1;
    ks2 = ka2;
    alpha1 = 5;
    alpha2 = alpha1;
}

glm::vec3 CheckerPlane::orthogonal(glm::vec3 n) const {
    if (n[2] != 0) return glm::vec3(0,1,0);
    return glm::normalize(glm::vec3(-n[1], n[0], 0));
}

glm::vec3 CheckerPlane::orthogonal(glm::vec3 v1, glm::vec3 v2) const {
    return cross(v1,v2);
}

bool CheckerPlane::hit(const Ray &r, float t_min, float t_max,
        hit_record &rec, Light &l) const {
    bool ret = Plane::hit(r,t_min,t_max,rec,l);
    if (!ret) return ret;

    glm::vec3 orth = orthogonal(normal);
    int ratio1 = floor(dot(rec.p - point, orth/tile_size));

    orth = orthogonal(normal, orth);
    int ratio2 = floor(dot(rec.p - point, orth/tile_size));

    if ((ratio1+ratio2)%2 == 0) {
        rec.ka = ka1;
        rec.kd = kd1;
        rec.ks = ks1;
        rec.alpha = alpha1;
    }
    else {
        rec.ka = ka2;
        rec.kd = kd2;
        rec.ks = ks2;
        rec.alpha = alpha2;
    }

    return ret;
}
