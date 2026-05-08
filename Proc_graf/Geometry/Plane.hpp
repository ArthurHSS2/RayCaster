#ifndef PLANE_H
#define PLANE_H

#include "../Geometry/Shape.hpp"

class Plane : public Shape{
    Point3d point;
    Vector3d normal;
    

    public:
        float R, G, B;

        Plane(Point3d p, Vector3d n, float R, float G, float B) : point(p), normal(n.normalizacao()), R(R), G(G), B(B) {
            material.R = R; material.G = G; material.B = B;
        }
        
        double intersect(const Ray& ray) const override;

        // Point parameter not used but necessary for Polymorphism
        Vector3d get_normal(const Point3d& p) const override;

        std::string get_type() const override {return "Plane";}

};

#endif