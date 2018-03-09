#include "Triangle.h"
#include <iostream>

Triangle::Triangle() : A(glm::vec3(0,0,0)), B(glm::vec3(0,0,0)),
    C(glm::vec3(0,0,0)) {}

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) : A(a), B(b), C(c) {}

bool Triangle::hit(const Ray &r, float t_min, float t_max, hit_record &rec,
        Light &l) const {
    (void)l;

    glm::vec3 edge1, edge2, n, h, s, q;
    float a, f, u, v;
    edge1 = B-A;
    edge2 = C-A;
    n = cross(edge1,edge2); // vector normal to plane of triangle

    h = cross(r.direction(), edge2);
    a = dot(edge1, h);
    if (a < 0.0001f && a > -0.0001f) return false;

    f = 1/a;
    s = r.origin() - A;
    u = f*dot(s,h);
    if (u < 0.0f || u > 1.0f) return false;
    q = cross(s,edge1);
    v = f*dot(r.direction(),q);
    if (v < 0.0f || u+v > 1.0f) return false;

    float t = f*dot(edge2,q);

    if (t < t_max && t > t_min) {
        rec.t = t;
        rec.p = r.location(rec.t);
        rec.normal = glm::normalize(n);
        rec.ka = ka;
        rec.kd = kd;
        rec.ks = ks;
        rec.alpha = alpha;
        return true;
    }
    return false;
}
