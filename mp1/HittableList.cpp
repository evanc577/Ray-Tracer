#include "HittableList.h"

HittableList::HittableList() {
    max_val = 1;
}

HittableList::HittableList(std::vector<Hittable *> l) {
    list_ = l;
    max_val = 1;
}

bool HittableList::hit(const Ray &r, float t_min, float t_max,
        hit_record &rec, Light &l) const {
    hit_record temp;
    bool hit = false;
    float closest = t_max;
    for (unsigned long i = 0; i < list_.size(); i++) {
        if (list_[i]->hit(r,t_min,closest, temp, l)) {
            hit = true;
            closest = temp.t;
            rec = temp;
        }
    }
    return hit;
}

vec3 HittableList::color(hit_record &rec, Light &l, const vec3 &v) {
    (void)v;
    vec3 color;
    vec3 direction;
    l.AtPoint(rec.p, color, direction);

    vec3 ambient = rec.ka*l.ia;

    vec3 diffuse = color*rec.kd*(dot(direction,rec.normal));
    if (isnan(diffuse.x()) || isnan(diffuse.y()) || isnan(diffuse.z())) {
        diffuse = vec3(0,0,0);
    }
    if (diffuse.x() < 0 || diffuse.y() < 0 || diffuse.z() < 0) {
        diffuse = vec3(0,0,0);
    }

    vec3 temp = ambient + diffuse;
    max_val = fmax(max_val, temp.x());
    max_val = fmax(max_val, temp.y());
    max_val = fmax(max_val, temp.z());

    return temp;
}
