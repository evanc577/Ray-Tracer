#include "Triangle.h"
#include <iostream>

Triangle::Triangle() : A(glm::vec3(0,0,0)), B(glm::vec3(0,0,0)),
    C(glm::vec3(0,0,0)) {}

Triangle::Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c) : A(a), B(b), C(c) {}

bool Triangle::hit(const Ray &r, float t_min, float t_max, hit_record &rec,
        Light &l) const {
    (void)l;
    
    glm::vec3 temp1 = B-A;
    glm::vec3 temp2 = C-A;
    glm::vec3 n = glm::normalize(cross(temp1, temp2));
    float D = dot(n,A);
    float t = (dot(n,r.origin()) + D)/dot(n,r.direction());
    if (t < 0) t *= -1;
    glm::vec3 p = r.origin() + t*r.direction();

    glm::vec3 edge0 = B-A;
    glm::vec3 edge1 = C-B;
    glm::vec3 edge2 = A-C;
    glm::vec3 C0 = p-A;
    glm::vec3 C1 = p-B;
    glm::vec3 C2 = p-C;
    
    if (dot(n,cross(edge0,C0)) < 0) return false;
    if (dot(n,cross(edge1,C1)) < 0) return false;
    if (dot(n,cross(edge2,C2)) < 0) return false;
    float temp = dot(n, r.direction());
    if (temp < 0.0001f && temp > -0.0001f) return false;
    if (t < t_max && t > t_min) {
        rec.t = t;
        rec.p = r.location(rec.t);
        rec.normal = glm::normalize(n);
        rec.ka = ka;
        rec.kd = kd;
        return true;
    }
    return false;
}
