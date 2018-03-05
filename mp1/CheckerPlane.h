#ifndef CHECKER_PLANE_H
#define CHECKER_PLANE_H

#include <cmath>
#include "Plane.h"

class CheckerPlane : public Plane {
    public:
        CheckerPlane() : Plane() {
            ka1 = vec3(1,1,1);
            ka2 = vec3(0,0,0);
            kd1 = ka1;
            kd2 = ka2;
        }
        CheckerPlane(vec3 p, vec3 n) : Plane(p,n) {
            ka1 = vec3(1,1,1);
            ka2 = vec3(0,0,0);
            kd1 = ka1;
            kd2 = ka2;
        }

        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec, Light &l) const;

        vec3 orthogonal(vec3 n) const;
        vec3 orthogonal(vec3 v1, vec3 v2) const;
        vec3 ka1;
        vec3 ka2;
        vec3 kd1;
        vec3 kd2;
        float tile_size;
};

vec3 CheckerPlane::orthogonal(vec3 n) const {
    if (n.z() != 0) return vec3(0,1,0);
    return unit_vector(vec3(-n.y(), n.x(), 0));
}

vec3 CheckerPlane::orthogonal(vec3 v1, vec3 v2) const {
    return cross(v1,v2);
}


bool CheckerPlane::hit(const Ray &r, float t_min, float t_max,
        hit_record &rec, Light &l) const {
    bool ret = Plane::hit(r,t_min,t_max,rec,l);
    if (!ret) return ret;

    vec3 orth = orthogonal(normal);
    int ratio1 = floor(dot(rec.p - point, orth/tile_size));

    orth = orthogonal(normal, orth);
    int ratio2 = floor(dot(rec.p - point, orth/tile_size));

    if ((ratio1+ratio2)%2 == 0) {
        rec.ka = ka1;
        rec.kd = kd1;
    }
    else {
        rec.ka = ka2;
        rec.kd = kd2;
    }

    return ret;
}
#endif
