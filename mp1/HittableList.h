#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <cmath>
#include <vector>
#include "Hittable.h"
#include "Light.h"

class HittableList : public Hittable {
    public:
        HittableList();
        HittableList(std::vector<Hittable *> l);

        float max_val;

        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec, Light &l) const;

        virtual glm::vec3 color(hit_record &rec, Light &l, const glm::vec3 &v);


        std::vector<Hittable *> list_;
};


#endif
