#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "commom.hpp"

class sphere : public hittable {
  public:
    sphere(const Point3d& center, double radius, const color& ka, const color& kd, const color& ks, double ns, const color& kr, const color& kt, double ni) 
        : center(center), radius(std::fmax(0,radius)), ka(ka), kd(kd), ks(ks), ns(ns), kr(kr), kt(kt), ni(ni) {}

    bool hit(const Ray& r, interval ray_t, hit_record& rec) const override {
        Vector3d oc = center - r.get_origin();
        auto a = r.get_dir().modulo_quadrado();
        auto h = r.get_dir().produto_escalar(oc);
        auto c = oc.modulo_quadrado() - radius*radius;

        auto discriminant = h*h - a*c;
        if (discriminant < 0)
            return false;

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        rec.normal = (rec.p - center) / radius;

	    // Passando os materiais para o hit_record
        rec.ka = ka;
        rec.kd = kd;
        rec.ks = ks;
        rec.ns = ns;
        rec.kr = kr;
        rec.kt = kt;
        rec.ni = ni;

        return true;
    }

  private:
    Point3d center;
    double radius;
    color ka; 
    color kd; 
    color ks;
    color kr;
    color kt;
    double ni; 
    double ns;
};

#endif
