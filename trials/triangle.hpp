#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "commom.hpp"
#include "plane.hpp"

class triangle : public hittable
{
private:
    Point3d a, b, c;
    Vector3d na, nb, nc;
    color ka; color kd; color ks; double ns;

public:
    // Removi as referências (&) nos Vector3d para evitar que o C++ perca os dados de memória
    triangle(Point3d a, Point3d b, Point3d c, Vector3d na, Vector3d nb, Vector3d nc, const color& ka, const color& kd, const color& ks, double ns)
      : a(a), b(b), c(c), na(na), nb(nb), nc(nc), ka(ka), kd(kd), ks(ks), ns(ns) {}
    
    bool hit(const Ray& r, interval ray_t, hit_record& rec) const override 
    {
        // 1. Mudámos para DOUBLE. Precisão máxima!
        const double epsilon = 1e-8; 

        Vector3d edge1 = b - a;
        Vector3d edge2 = c - a;
        Vector3d ray_vector = r.get_dir();

        Vector3d ray_cross_e2 = ray_vector.produto_vetorial(edge2);
        double det = edge1.produto_escalar(ray_cross_e2);

        // 2. Fuga à armadilha do abs()! Testamos matematicamente sem funções externas
        if (det > -epsilon && det < epsilon) return false; 

        double inv_det = 1.0 / det;
        Vector3d s = r.get_origin() - a;
        double u = inv_det * s.produto_escalar(ray_cross_e2);

        if (u < -epsilon || u > 1.0 + epsilon) return false; 

        Vector3d s_cross_e1 = s.produto_vetorial(edge1);
        double v = inv_det * ray_vector.produto_escalar(s_cross_e1);

        if (v < -epsilon || u + v > 1.0 + epsilon) return false; 

        double t = inv_det * edge2.produto_escalar(s_cross_e1);

        // 3. Bloqueio de Shadow Acne e Triângulos Fantasmas
        if (t <= ray_t.min || t >= ray_t.max) {
            return false;
        }

        rec.t = t;
        rec.p = r.at(rec.t);

        double w = 1.0 - u - v; 
        
        Vector3d normal_interpolada = (na * w) + (nb * v) + (nc * u);
        
        // Proteção contra divisões por zero nas normais
        if (normal_interpolada.modulo() > epsilon) {
            rec.normal = normal_interpolada.normalizacao();
        } else {
            rec.normal = Vector3d(0, 1, 0); // Normal de emergência
        }
        
        // Vira a normal se batermos nas "costas" do triângulo
        if (rec.normal.produto_escalar(ray_vector) > 0) {
            rec.normal = rec.normal * -1.0; 
        }

        rec.ka = ka;
        rec.kd = kd;
        rec.ks = ks;
        rec.ns = ns;
        
        return true;
    }
};

#endif