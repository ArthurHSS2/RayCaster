#ifndef RAY_H
#define RAY_H

#include "math/Vector3d.hpp"


class Ray {
    Point3d origin;
    Vector3d direction;

    public:
        Ray(const Point3d& o, const Vector3d& d) : origin(o), direction(d.normalizacao()) {}

        // P(t) = O + t*D -> Onde o raio está na distância t?
        Point3d at(double t) const;

        Point3d get_origin() const;

        Vector3d get_dir() const;
};

#endif
