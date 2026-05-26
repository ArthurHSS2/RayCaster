#ifndef HITTABLE_H
#define HITTABLE_H

#include "commom.hpp"

class hit_record {
  public:
    Point3d p;
    Vector3d normal;
    double t;
    bool front_face;
    color col;
    
        void set_face_normal(const Ray& r, const Vector3d& outward_normal) {
            // Sets the hit record normal vector.
            // NOTE: the parameter `outward_normal` is assumed to have unit length.
    
            front_face = r.get_dir().produto_escalar(outward_normal) < 0;
            normal = front_face ? outward_normal : -outward_normal;
        }
};

class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const Ray& r, interval ray_t, hit_record& rec) const = 0;
};

#endif
