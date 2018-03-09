#include "HittableList.h"
#include <iostream>

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

glm::vec3 HittableList::color(hit_record &rec, Light &l, const glm::vec3 &d) {

    // calculate and return a list of ambient, directional, specular, and
    // light directions at rec.p
    std::vector<glm::vec3> ca_list;
    std::vector<glm::vec3> cd_list;
    std::vector<glm::vec3> cs_list;
    std::vector<glm::vec3> light_d_list;
    l.AtPoint(rec.p, ca_list, cd_list, cs_list, light_d_list);

    // Phong model values
    std::vector<glm::vec3> r_list;  // R vector
    std::vector<glm::vec3> a_list;  // ambient term
    std::vector<glm::vec3> d_list;  // diffuse term
    std::vector<glm::vec3> s_list;  // specular term

    int size = ca_list.size();
    r_list.reserve(size);
    a_list.reserve(size);
    d_list.reserve(size);
    s_list.reserve(size);

    for (int i = 0; i < size; i++) {
        // add R vectors to r_list
        r_list.push_back(light_d_list[i] -
                2*(dot(light_d_list[i], rec.normal))*rec.normal);

        // add ambient term to a_list
        a_list.push_back(ca_list[i]*rec.ka);
    }

    // determines if specular term should be added
    std::vector<bool> pos_diffuse_list;

    // add diffuse term to d_list;
    for (int i = 0; i < size; i++) {
        pos_diffuse_list.push_back(false);
        glm::vec3 temp_diffuse = cd_list[i]*rec.kd*
            dot(light_d_list[i], rec.normal);

        if (glm::isnan(temp_diffuse[0]) || glm::isnan(temp_diffuse[1]) ||
                glm::isnan(temp_diffuse[2])) {
            temp_diffuse = glm::vec3(0,0,0);
            pos_diffuse_list[i] = false;
        }
        pos_diffuse_list[i] = true;
        if (temp_diffuse[0] < 0) temp_diffuse[0] = 0;
        if (temp_diffuse[1] < 0) temp_diffuse[1] = 0;
        if (temp_diffuse[2] < 0) temp_diffuse[2] = 0;

        if (temp_diffuse[0] <= 0.0001f && temp_diffuse[1] <= 0.0001f &&
                temp_diffuse[2] < 0.0001f) {
            pos_diffuse_list[i] = false;
        }
        d_list.push_back(temp_diffuse);
    }

    // add specular term to s_list
    for (int i = 0; i < size; i++) {
        glm::vec3 temp_d = -d;
        if (pos_diffuse_list[i]) {
            glm::vec3 specular(0,0,0);
            specular = rec.ks*float((pow(-1.0f*
                            dot(r_list[i],temp_d),rec.alpha)))*cs_list[i];
            if (glm::isnan(specular[0]) || glm::isnan(specular[1]) ||
                    glm::isnan(specular[2])) {
                specular = glm::vec3(0,0,0);
            }
            if (specular[0] < 0) specular[0] = 0;
            if (specular[1] < 0) specular[1] = 0;
            if (specular[2] < 0) specular[2] = 0;

            s_list.push_back(specular);
        }
        else {
            s_list.push_back(glm::vec3(0,0,0));
        }

    }

    glm::vec3 temp(0,0,0);
    for (int i = 0; i < size; i++) {
        temp += a_list[i];
    }
    for (int i = 0; i < size; i++) {
        temp += d_list[i];
    }
    for (int i = 0; i < size; i++) {
        temp += s_list[i];
    }
    max_val = fmax(max_val, temp[0]);
    max_val = fmax(max_val, temp[1]);
    max_val = fmax(max_val, temp[2]);

    return temp;
}
