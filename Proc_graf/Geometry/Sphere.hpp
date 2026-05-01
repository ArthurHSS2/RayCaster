#include "../Geometry/Shape.hpp"

#ifndef SPHERE_H

#define SPHERE_H

class Sphere : public Shape{
    Point3d center;
    double   radius;
    double R, G, B; //ajeitar RGB

    public:
        Sphere(Point3d c, double r, double R, double G, double B) : center(c), radius(r), R(R), G(G), B(B) {}

        // retorna t > 0 se houver interseção, -1 caso contrário
        double intersect(const Ray& ray) const override;

        Vector3d get_normal(const Point3d& p) const override;

        Point3d get_center();

        double get_radius();

        std::string get_type() const {return "Sphere";}
};

#endif