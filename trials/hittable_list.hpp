#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include "commom.hpp"
#include <vector>


class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
    }

    bool hit(const Ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        
        // Esta é a variável mágica: guarda a distância do objeto mais próximo até agora
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            // O SEGREDO: Em vez de passar 'ray_t.max', passamos o 'closest_so_far'.
            // Assim, o próximo objeto só será aceite se estiver mais perto do que o anterior!
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t; // Atualiza o novo recorde de proximidade
                rec = temp_rec;              // Grava os dados da cor e normal
            }
        }

        return hit_anything;
    }
};

#endif
