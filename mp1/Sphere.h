#ifndef SPHERE_H
#define SPHERE_H

#include "Hittable.h"

class Sphere : public Hittable {
    public:
        Sphere() : center(vec3(0,0,0)), radius(1) {}
        Sphere(vec3 c, float r) : center(c), radius(r) {}
        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec, Light &l) const;
        vec3 center;
        float radius;
};

bool Sphere::hit(const Ray& r, float t_min, float t_max, hit_record& rec,
        Light &l) const {
    (void)l;
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
            // rec.ka = vec3(0,0,1);
            rec.ka = ka;
            rec.kd = kd;
            return true;
        }
        temp = (-b + sqrt(discriminant))/(a);
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.location(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.ka = ka;
            rec.kd = kd;
            // rec.ka = 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1,
                    // rec.normal.z()+1);
            // rec.kd = vec3(1,1,1);
            rec.normal *= -1;
            return true;
        }
    }
    return false;
}

#endif
