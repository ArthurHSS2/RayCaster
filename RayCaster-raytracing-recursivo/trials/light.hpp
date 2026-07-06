#ifndef LIGHT_H
#define LIGHT_H

#include "math/Vector3d.hpp"
#include "color.hpp"

struct PointLight {
    Point3d pos;
    color intensity;

    // Construtor padrão
    PointLight(const Point3d& p, const color& c) : pos(p), intensity(c) {}
};

#endif