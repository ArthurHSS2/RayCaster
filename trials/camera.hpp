#ifndef CAMERA_H
#define CAMERA_H

#include <cmath>
#include "color.hpp"
#include "hittable.h"
#include <vector>
#include "light.hpp"

class camera {
  public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width  = 100;  // Rendered image width in pixel count
    int    image_height;        // Rendered image height
    
    int    max_depth    = 5;    // [NOVO] Limite de recursão (quiques do raio)

    double vfov = 90.0;  // Ângulo de visão vertical (Zoom)
    Point3d lookfrom = Point3d(0, 0, 0);   // Posição da câmara
    Point3d lookat   = Point3d(0, 0, -1);  // Para onde ela aponta
    Vector3d vup     = Vector3d(0, 1, 0);  // Qual é a direção "para cima"

    // O render recebe a luz da cena e a luz ambiente global
    void render(const hittable& world, const std::vector<PointLight>& lights, const color& global_ambient) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                auto pixel_center = pixel00_loc + (pixel_delta_u * i) + (pixel_delta_v * j);
                auto ray_direction = pixel_center - center;
                Ray r(center, ray_direction);

                // Passamos o 'max_depth' para iniciar a contagem da recursão
                color pixel_color = ray_color(r, max_depth, world, lights, global_ambient);
                write_color(std::cout, pixel_color);
            }
        }    
        std::clog << "\rDone.                \n";
    }

  private:
    
    Point3d center;         // Camera center
    Point3d pixel00_loc;    // Location of pixel 0, 0
    Vector3d   pixel_delta_u;  // Offset to pixel to the right
    Vector3d   pixel_delta_v;  // Offset to pixel below
    Vector3d   u, v, w;

    // --- FUNÇÕES MATEMÁTICAS AUXILIARES PARA A ENTREGA 4 ---
    
    // Calcula a direção do raio refletido (Espelho)
    Vector3d reflect(const Vector3d& v, const Vector3d& n) const {
        return v - (n * 2.0 * v.produto_escalar(n));
    }

    // Calcula a direção do raio refratado (Vidro / Lei de Snell)
    Vector3d refract(const Vector3d& uv, const Vector3d& n, double etai_over_etat) const {
        double cos_theta = std::min((-uv).produto_escalar(n), 1.0);
        Vector3d r_out_perp =  (uv + (n * cos_theta)) * etai_over_etat;
        double r_out_perp_length_sq = r_out_perp.produto_escalar(r_out_perp);
        Vector3d r_out_parallel = n * (-std::sqrt(std::abs(1.0 - r_out_perp_length_sq)));
        return r_out_perp + r_out_parallel;
    }

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = lookfrom; // A câmara é movida para o lookfrom

        // 1. Calcular o Field of View (Zoom)
        auto theta = vfov * M_PI / 180.0;
        auto h = std::tan(theta/2);
        
        // A distância focal agora baseia-se na distância entre a câmara e o alvo
        auto focal_length = (lookfrom - lookat).modulo(); 
        
        auto viewport_height = 2.0 * h * focal_length;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // 2. Calcular a Orientação da Câmara no espaço 3D usando produto vetorial
        w = (lookfrom - lookat).normalizacao();     // Eixo Z local da câmara
        u = vup.produto_vetorial(w).normalizacao(); // Eixo X local da câmara (Direita)
        v = w.produto_vetorial(u);                  // Eixo Y local da câmara (Cima)

        // 3. Os vetores do ecrã agora inclinam-se junto com a câmara
        auto viewport_u = u * viewport_width;
        auto viewport_v = -v * viewport_height;

        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // 4. Posicionar o ecrã na frente da câmara inclinada
        auto viewport_upper_left = center - (w * focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5;
    }

    color ray_color(const Ray& r, int depth, const hittable& world, const std::vector<PointLight>& lights, const color& global_ambient) const {
        // 1. LIMITE DE RECURSÃO (Failsafe)
        if (depth <= 0) {
            return color(0, 0, 0); // Excedeu o limite de quiques da luz
        }
        hit_record rec;

        // Se o raio bateu em algo (intervalo modificado de 0 para 0.001)
        if (world.hit(r, interval(0.001, infinity), rec)) {
            
            // --- 1. ILUMINAÇÃO DIRETA (PHONG DA ENTREGA 3) ---
            color cor_final = color(rec.ka.x * global_ambient.x, 
                                    rec.ka.y * global_ambient.y, 
                                    rec.ka.z * global_ambient.z);

            Vector3d N = rec.normal.normalizacao();
            Vector3d V = (-r.get_dir()).normalizacao(); // Direção para a câmara (Inverso do raio incidente)
            Vector3d I = r.get_dir().normalizacao();    // Direção incidente real

            // Loop por todas as luzes da cena (Phong)
            for (const auto& light : lights) {
                Vector3d dir_luz = light.pos - rec.p;
                double dist_luz = dir_luz.modulo();
                Vector3d L = dir_luz.normalizacao();

                Ray shadow_ray(rec.p + (N * 0.001), L);
                hit_record shadow_rec;
                
                if (!world.hit(shadow_ray, interval(0.001, dist_luz), shadow_rec)) {
                    double fator_difuso = std::max(0.0, L.produto_escalar(N));
                    color difusa = color(rec.kd.x * light.intensity.x, 
                                         rec.kd.y * light.intensity.y, 
                                         rec.kd.z * light.intensity.z) * fator_difuso;

                    Vector3d R = (N * 2.0 * L.produto_escalar(N)) - L;
                    double fator_especular = std::pow(std::max(0.0, R.produto_escalar(V)), rec.ns);
                    color especular = color(rec.ks.x * light.intensity.x, 
                                            rec.ks.y * light.intensity.y, 
                                            rec.ks.z * light.intensity.z) * fator_especular;

                    cor_final = cor_final + difusa + especular;
                }
            }

            // --- 2. ILUMINAÇÃO RECURSIVA (ENTREGA 4) ---
            color cor_reflexao(0, 0, 0);
            color cor_refracao(0, 0, 0);

            // A. REFLEXÃO (Se o material tiver kr > 0)
            if (rec.kr.x > 0.0 || rec.kr.y > 0.0 || rec.kr.z > 0.0) {
                Vector3d refletida = reflect(I, N);
                // Dispara o novo raio com um pequeno Epsilon para fora do objeto
                Ray raio_refletido(rec.p + (N * 0.001), refletida);
                
                // Chamada recursiva (note o depth - 1)
                cor_reflexao = ray_color(raio_refletido, depth - 1, world, lights, global_ambient);
            }

            // B. REFRAÇÃO / TRANSPARÊNCIA (Se o material tiver kt > 0)
            if (rec.kt.x > 0.0 || rec.kt.y > 0.0 || rec.kt.z > 0.0) {
                // Descobre se o raio está a entrar no material ou a sair para o ar
                double cos_theta = std::min((-I).produto_escalar(N), 1.0);
                bool entering = (I.produto_escalar(N) < 0);
                
                Vector3d calc_normal = entering ? N : (N * -1.0);
                // Assume que o índice do ar é 1.0
                double razao_refracao = entering ? (1.0 / rec.ni) : (rec.ni / 1.0);
                
                // Verifica a Reflexão Interna Total (TIR) - Exemplo: ver por baixo da água
                double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
                if (razao_refracao * sin_theta > 1.0) {
                    // Se não pode refratar, reflete tudo
                    Vector3d refletida = reflect(I, calc_normal);
                    Ray raio_refletido(rec.p + (calc_normal * 0.001), refletida);
                    cor_refracao = ray_color(raio_refletido, depth - 1, world, lights, global_ambient);
                } else {
                    // Refrata de acordo com a Lei de Snell
                    Vector3d refratada = refract(I, calc_normal, razao_refracao);
                    // Como o raio entra no objeto, aplicamos o Epsilon no sentido oposto à normal externa
                    Ray raio_refratado(rec.p - (calc_normal * 0.001), refratada);
                    cor_refracao = ray_color(raio_refratado, depth - 1, world, lights, global_ambient);
                }
            }

            // C. COMBINAÇÃO FINAL
            // Multiplicamos os resultados obtidos pelas constantes do material
            color parte_refletida = color(rec.kr.x * cor_reflexao.x, rec.kr.y * cor_reflexao.y, rec.kr.z * cor_reflexao.z);
            color parte_refratada = color(rec.kt.x * cor_refracao.x, rec.kt.y * cor_refracao.y, rec.kt.z * cor_refracao.z);

            return cor_final + parte_refletida + parte_refratada;
        }

        // Se não bateu em nada (Background)
        return color(0, 0, 0);
    }
};

#endif