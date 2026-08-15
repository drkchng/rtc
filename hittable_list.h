#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "main_utility.h"

#include "hittable.h"

#include <vector>


class hittable_list: public hittable {
   public:
      std::vector<shared_ptr<hittable>> objects;

      hittable_list() {}
      hittable_list(shared_ptr<hittable> object) { add(object); }

      void clear() { objects.clear(); }

      void add(shared_ptr<hittable> object) {
         objects.push_back(object);
      }

      bool hit(const ray& r, interval r_t, hit_record& rec) const override {
         hit_record temp_rec;
         bool hit_anything = false;
         auto closest_so_far = r_t.max;

         for (const auto& object : objects) {
            if (object->hit(r, interval(r_t.min, closest_so_far), temp_rec)) {
               hit_anything = true;
               closest_so_far = temp_rec.t;
               rec = temp_rec;
            }
         }

         return hit_anything;
      }
};

#endif