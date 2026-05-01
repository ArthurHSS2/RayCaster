#include "../Geometry/Shape.hpp"

#ifndef PLANE_H

#define PLANE_H

class Plane : public Shape{
    Point3d point;
    Vector3d normal;
    int R, G, B;

    public:
        Plane(Point3d p, Vector3d n, int R, int G, int B) 
            : point(p), normal(n.normalizacao()), R(R), G(G), B(B) {}
        
        double intersect(const Ray& ray) const override;

        // Point parameter not used but necessary for Polymorphism
        Vector3d get_normal(const Point3d& p) const override;

        std::string get_type() const {return "Plane";}

};

#endif