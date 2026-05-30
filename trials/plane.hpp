#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"
#include "commom.hpp"

class plane : public hittable {
  public:
    // O construtor recebe um ponto pertencente ao plano, o vetor normal e a cor
    // Construtor atualizado com os materiais
    plane(const Point3d& point, const Vector3d& normal, const color& ka, const color& kd, const color& ks, double ns) 
        : point(point), normal(normal.normalizacao()), ka(ka), kd(kd), ks(ks), ns(ns) {}

    bool hit(const Ray& r, interval ray_t, hit_record& rec) const override {
        // Produto escalar entre a direção do raio e a normal do plano (o denominador)
        double denom = r.get_dir().produto_escalar(normal);
        
        // Verifica se o raio é quase paralelo ao plano usando um Epsilon (1e-8)
        if (std::abs(denom) < 1e-8)
            return false;

        // Calcula a distância 't' da interseção
        Vector3d p0l0 = point - r.get_origin();
        double t = p0l0.produto_escalar(normal) / denom;

        // O 'interval' substitui aquelas antigas verificações (t < 0.001)
        // Ele checa se o 't' está dentro da faixa visível e segura da câmera
        if (!ray_t.surrounds(t))
            return false;

        // Preenche o "caderno de anotações" (hit_record) com os dados do impacto
        rec.t = t;
        rec.p = r.at(rec.t);
        // Passando os materiais para o hit_record
        rec.ka = ka;
        rec.kd = kd;
        rec.ks = ks;
        rec.ns = ns;
        
        
        // Essa função da classe base decide se o raio bateu 
        // no plano pela frente ou por trás, e ajusta a normal adequadamente
        rec.set_face_normal(r, normal);

        return true;
    }

  private:
    Point3d point;
    Vector3d normal;
    color ka; 
    color kd; 
    color ks; 
    double ns;
};

#endif