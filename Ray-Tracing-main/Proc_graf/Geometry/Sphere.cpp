#include "Sphere.hpp"

double Sphere::intersect(const Ray& ray) const 
{
    Vector3d oc = ray.get_origin() - center; // Vetor da esfera para a câmera
    
    // Coeficientes da equação at² + bt + c = 0
    double a = ray.get_dir().produto_escalar(ray.get_dir());
    double b = 2.0f * oc.produto_escalar(ray.get_dir());
    double c = oc.produto_escalar(oc) - radius * radius;
    
    double delta = b*b - 4*a*c;

    if (delta < 0) return -1.0f; // Errou a esfera
    
    // Retorna a distância t (Point de impacto)
    return (-b - std::sqrt(delta)) / (2.0f * a);
}

Vector3d Sphere::get_normal(const Point3d& p) const 
{
    return (p-center)/radius;
}

Point3d Sphere::get_center() 
{
    return center;    
}

double Sphere::get_radius() 
{
    return radius;
}