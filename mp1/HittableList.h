#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <vector>
#include "Hittable.h"

class HittableList : public Hittable {
    public:
        HittableList() {}
        HittableList(std::vector<Hittable *> l) {
            list_ = l;
        }

        virtual bool hit(const Ray &r, float t_min, float t_max,
                hit_record &rec) const {
            hit_record temp;
            bool hit = false;
            float closest = t_max;
            for (unsigned long i = 0; i < list_.size(); i++) {
                if (list_[i]->hit(r,t_min,closest, temp)) {
                    hit = true;
                    closest = temp.t;
                    rec = temp;
                }
            }
            return hit;
        }

        virtual vec3 color(hit_record &rec) const {
            return rec.color;
        }

        std::vector<Hittable *> list_;
};


#endif
