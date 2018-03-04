#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"

class Sphere : public Hittable {
    public:
        Sphere() : center(vec3(0,0,0)), radius(1) {}
        Sphere(vec3 c, float r) : center(c), radius(r) {}
        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec) const;
        vec3 center;
        float radius;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b - a*c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant))/(a);
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.location(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = (-b + sqrt(discriminant))/(a);
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.location(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}
#endif
