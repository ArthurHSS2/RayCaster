#include "../Core/Ray.hpp"

#ifndef SHAPE_H

#define SHAPE_H

class Shape 
{
    public:
        virtual ~Shape() = default;
        virtual double intersect(const Ray& ray) const = 0;
        virtual Vector3d get_normal(const Point3d& p) const = 0;
        virtual std::string get_type() const = 0;
};

#endif