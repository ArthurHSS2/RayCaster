#include "Triangle.hpp"

double Triangle::intersect(const Ray& ray) const 
{
    Vector3d edge1 = v2 - v1;
    Vector3d edge2 = v3 - v1;

    const Vector3d normal = edge1.produto_vetorial(edge2);

    
}
Vector3d Triangle::get_normal(const Point3d& p) const
{

}