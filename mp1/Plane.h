#ifndef PLANE_H
#define PLANE_H

#include "Hittable.h"

class Plane : public Hittable {
    public:
        Plane() : point(vec3(0,0,0)), normal(vec3(0,0,1)) {}
        Plane(vec3 p, vec3 n) : point(p), normal(unit_vector(n)) {}
        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec) const;
        vec3 point;
        vec3 normal;
};


bool Plane::hit(const Ray &r, float t_min, float t_max, hit_record &rec) const {
    float temp = dot(normal, r.direction());
    if (temp > 0.0001f || temp < -0.0001f) {
        float t = dot((point - r.origin()), normal) / temp;
        if (t < t_max && t > t_min) {
            rec.t = t;
            rec.p = r.location(rec.t);
            rec.normal = normal;
            rec.color = 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1,
                    rec.normal.z()+1);
            return true;
        }
    }
    return false;
}

#endif
