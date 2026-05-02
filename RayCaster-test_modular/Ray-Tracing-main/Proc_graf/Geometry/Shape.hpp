#ifndef SHAPE_H
#define SHAPE_H

#include "../Core/Ray.hpp"
#include <map>
#include <string>

struct Material {
    float R, G, B;
};

class Shape 
{
    public:
        Material material;
        virtual ~Shape() = default;
        virtual double intersect(const Ray& ray) const = 0;
        virtual Vector3d get_normal(const Point3d& p) const = 0;
        virtual std::string get_type() const = 0;
};

#endif