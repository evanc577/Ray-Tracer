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
    glm::vec3 ca;
    glm::vec3 cd;
    glm::vec3 cs;
    glm::vec3 direction;
    l.AtPoint(rec.p, ca, cd, cs, direction);

    glm::vec3 r = direction - 2*(dot(direction, rec.normal))*rec.normal;
    glm::vec3 temp_v = -v;

    glm::vec3 ambient = rec.ka*ca;

    glm::vec3 diffuse = cd*rec.kd*(dot(direction,rec.normal));
    if (glm::isnan(diffuse[0]) || glm::isnan(diffuse[1]) ||
                glm::isnan(diffuse[2])) {
        diffuse = glm::vec3(0,0,0);
    }
    bool do_specular = true;
    if (diffuse[0] <= 0) diffuse[0] = 0;
    if (diffuse[1] <= 0) diffuse[1] = 0;
    if (diffuse[2] <= 0) diffuse[2] = 0;
    if (diffuse[0] <= 0 && diffuse[1] <= 0 && diffuse[2] <= 0) do_specular = false;

    glm::vec3 specular(0,0,0);
    if (do_specular) {
        specular = rec.ks*float((pow(-1.0f*dot(r,temp_v),rec.alpha)))*cs;
        if (glm::isnan(specular[0]) || glm::isnan(specular[1]) ||
                glm::isnan(specular[2])) {
            diffuse = glm::vec3(0,0,0);
        }
        if (specular[0] < 0) specular[0] = 0;
        if (specular[1] < 0) specular[1] = 0;
        if (specular[2] < 0) specular[2] = 0;
    }

    glm::vec3 temp = ambient + diffuse + specular;
    max_val = fmax(max_val, temp[0]);
    max_val = fmax(max_val, temp[1]);
    max_val = fmax(max_val, temp[2]);

    return temp;
}
