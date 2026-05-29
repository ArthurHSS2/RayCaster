#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "commom.hpp"
#include "plane.hpp"

/*struct  triangle
{
    std::array<Point3d, 3> vertices; 
    //const color& ka; const color& kd; const color& ks; double ns;
}; */

class triangle : public hittable
{
private:
    Point3d a, b, c;
    Vector3d na, nb, nc;
    color ka; color kd; color ks; double ns;

public:
    triangle(Point3d a, Point3d b, Point3d c, Vector3d& na, Vector3d& nb, Vector3d& nc, const color& ka, const color& kd, const color& ks, double ns)
      : a(a), b(b), c(c), na(na), nb(nb), nc(nc), ka(ka), kd(kd), ks(ks), ns(ns) {}
    
    bool hit(const Ray& r, interval ray_t, hit_record& rec) const override 
    {
        constexpr float epsilon = std::numeric_limits<float>::epsilon();

        Vector3d edge1 = b - a;
        Vector3d edge2 = c - a;
        Vector3d ray_vector = r.get_dir();

        // Backface culling, assuming CCW-wound triangles.
        const Vector3d normal = edge1.produto_vetorial(edge2); // No need to normalize
        if (normal.produto_escalar(ray_vector) > 0) return false;

        Vector3d ray_cross_e2 = ray_vector.produto_vetorial(edge2);
        float det = edge1.produto_escalar(ray_cross_e2);

        if (abs(det) < epsilon) return false; // Ray is parallel to triangle

        float inv_det = 1.0 / det;
        Vector3d s = r.get_origin() - a;
        float u = inv_det * s.produto_escalar(ray_cross_e2);

        if (u < -epsilon || u - 1 > epsilon) return false; // Ray passes outside edge2's bounds

        Vector3d s_cross_e1 = s.produto_vetorial(edge1);
        float v = inv_det *ray_vector.produto_escalar(s_cross_e1);

        if (v < -epsilon || u + v - 1 > epsilon) return false; // Ray passes outside edge1's bounds

        // The ray line intersects with the triangle.
        // We compute t to find where on the ray the intersection is.
        float t = inv_det * edge2.produto_escalar(s_cross_e1);

        if (t >= ray_t.min && t <= ray_t.max) // Ray intersection
        {
            rec.t = t;
            rec.p = r.at(rec.t);

            // --- INTERPOLAÇÃO DE NORMAL ---
            float w = 1.0f - u - v; // Coordenada baricêntrica W (peso do vértice A)
            
            // Multiplicamos cada normal pelo seu peso e somamos tudo
            Vector3d normal_interpolada = (na * w) + (nb * v) + (nc * u);
            
            rec.normal = normal_interpolada.normalizacao();

            // Passando os materiais para o hit_record
            rec.ka = ka;
            rec.kd = kd;
            rec.ks = ks;
            rec.ns = ns;
            return  true;
        }
        else // This means that there is a line intersection but not a ray intersection.
            return false;
    }
};

#endif
