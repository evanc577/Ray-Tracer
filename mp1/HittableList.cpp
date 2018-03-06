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

glm::vec3 HittableList::color(hit_record &rec, Light &l, const glm::vec3 &v) {
    (void)v;
    glm::vec3 color;
    glm::vec3 direction;
    l.AtPoint(rec.p, color, direction);

    glm::vec3 ambient = rec.ka*l.ia;

    glm::vec3 diffuse = color*rec.kd*(dot(direction,rec.normal));
    if (glm::isnan(diffuse[0]) || glm::isnan(diffuse[1]) ||
                glm::isnan(diffuse[2])) {
        diffuse = glm::vec3(0,0,0);
    }
    if (diffuse[0] < 0) diffuse[0] = 0;
    if (diffuse[1] < 0) diffuse[1] = 0;
    if (diffuse[2] < 0) diffuse[2] = 0;

    glm::vec3 temp = ambient + diffuse;
    max_val = fmax(max_val, temp[0]);
    max_val = fmax(max_val, temp[1]);
    max_val = fmax(max_val, temp[2]);

    return temp;
}
