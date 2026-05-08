#include "Mesh.hpp"

double Mesh::intersect(const Ray& ray) const {
    double closest_t = 999999;
    bool found = false;
    for (int i = 0; i < lista.size(); i++)
    {
        const auto& triangle = lista[i];

        double epsilon = 1e-8;

        Vector3d edge1 = triangle[1] -   triangle[0];
        Vector3d edge2 = triangle[2] -  triangle[0];

        const Vector3d normal = edge1.produto_vetorial(edge2);
        if (ray.get_dir().produto_escalar(normal) > 0) continue;

        Vector3d ray_cross_e2 = ray.get_dir().produto_vetorial(edge2);
        double det = ray_cross_e2.produto_escalar(edge1);
        
        if (abs(det) < epsilon) continue;

        double inv_det = 1.0 / det;
        Vector3d s = ray.get_origin() - triangle[0];
        double u = inv_det * s.produto_escalar(ray_cross_e2);

        if (u < -epsilon || u - 1 > epsilon) continue;

        Vector3d s_cross_e1 = s.produto_vetorial(edge1);
        double v = inv_det * ray.get_dir().produto_escalar(s_cross_e1);

        if (v < -epsilon || u + v - 1 > epsilon) continue;

        double t = inv_det * edge2.produto_escalar(s_cross_e1);
        
        if (t > epsilon && t < closest_t) // Ray intersection
        {
            closest_t = t;
            found = true;
        }
    }
    if (found == true) 
    {
        return closest_t;
    } 
    else 
    {
        return -1;
    }
}

Vector3d Mesh::get_normal(const Point3d& p) const {
    return Vector3d(0, 0, 0);
}
