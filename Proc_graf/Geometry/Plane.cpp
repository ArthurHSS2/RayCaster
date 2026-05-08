#include "Plane.hpp"

double Plane::intersect(const Ray& ray) const {
    double denom = ray.get_dir().produto_escalar(normal);
    
    if (std::abs(denom) < 1e-6) return -1.0f; //Epsilon pra evitar problema com 0
    
    Vector3d p0_minus_origin = point - ray.get_origin();
    double t = p0_minus_origin.produto_escalar(normal) / denom;
    
    if (t < 0.001f) return -1.0f; // Epsilon
    
    return t;
}

Vector3d Plane::get_normal(const Point3d& p) const
{
    return normal;
}