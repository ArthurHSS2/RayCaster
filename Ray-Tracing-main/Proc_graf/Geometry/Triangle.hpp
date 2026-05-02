#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.hpp"
#include <vector>

class Triangle : public Shape 
{
    Point3d v1, v2, v3;
    Vector3d normal;

    public:
        Triangle(Point3d v1, Point3d v2, Point3d v3, Vector3d normal) : v1(v1), v2(v2), v3(v3), normal(normal) {}
        double intersect(const Ray& ray) const override;
        Vector3d get_normal(const Point3d& p) const override;
        std::string get_type() {return "Triangle";}
};

#endif