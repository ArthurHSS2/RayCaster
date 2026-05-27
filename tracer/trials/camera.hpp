#ifndef CAMERA_H
#define CAMERA_H

#include "color.hpp"
#include "hittable.h"
#include "light.hpp"

class camera {
  public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int    image_width  = 100;  // Rendered image width in pixel count

    // O render recebe a luz da cena e a luz ambiente global
    void render(const hittable& world, const PointLight& light, const color& global_ambient) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                auto pixel_center = pixel00_loc + (pixel_delta_u * i) + (pixel_delta_v * j);
                auto ray_direction = pixel_center - center;
                Ray r(center, ray_direction);

                // o raio, o mundo, a lâmpada e a cor ambiente são passados para o cálculo do pixel
                color pixel_color = ray_color(r, world, light, global_ambient);
                write_color(std::cout, pixel_color);
            }
        }    
        std::clog << "\rDone.                \n";
    }

  private:
    int    image_height;   // Rendered image height
    Point3d center;         // Camera center
    Point3d pixel00_loc;    // Location of pixel 0, 0
    Vector3d   pixel_delta_u;  // Offset to pixel to the right
    Vector3d   pixel_delta_v;  // Offset to pixel below

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        center = Point3d(0, 0, 0);

        // Determine viewport dimensions.
        auto focal_length = 1.0;
        auto viewport_height = 2.0;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        auto viewport_u = Vector3d(viewport_width, 0, 0);
        auto viewport_v = Vector3d(0, -viewport_height, 0);

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left =
            center - Vector3d(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = Point3d(0,0,0) + (viewport_upper_left + (pixel_delta_u + pixel_delta_v) * 0.5);
    }

    color ray_color(const Ray& r, const hittable& world, const PointLight& light, const color& global_ambient) const {
        hit_record rec;

        // Se o raio bateu em algo (intervalo modificado de 0 para 0.001)
        if (world.hit(r, interval(0.001, infinity), rec)) {
            
            // AMBIENTE (Multiplicação componente a componente x, y, z)
            color ambiente = color(rec.ka.x * global_ambient.x, 
                                   rec.ka.y * global_ambient.y, 
                                   rec.ka.z * global_ambient.z);

            Vector3d N = rec.normal.normalizacao();
            Vector3d dir_luz = light.pos - rec.p;
            double dist_luz = dir_luz.modulo();
            Vector3d L = dir_luz.normalizacao();
            Vector3d V = (-r.get_dir()).normalizacao();

            // DIFUSA
            double fator_difuso = std::max(0.0, L.produto_escalar(N));
            color difusa = color(rec.kd.x * light.intensity.x, 
                                 rec.kd.y * light.intensity.y, 
                                 rec.kd.z * light.intensity.z) * fator_difuso;

            // ESPECULAR
            Vector3d R = (N * 2.0 * L.produto_escalar(N)) - L;
            double fator_especular = std::pow(std::max(0.0, R.produto_escalar(V)), rec.ns);
            color especular = color(rec.ks.x * light.intensity.x, 
                                    rec.ks.y * light.intensity.y, 
                                    rec.ks.z * light.intensity.z) * fator_especular;

            // --- SOMBRAS ---
            // Dispara o raio de sombra a partir do ponto de impacto
            Ray shadow_ray(rec.p + (N * 0.001), L);
            hit_record shadow_rec;
            
            // Se o raio de sombra bater em qualquer coisa antes de chegar na luz, está na sombra
            if (world.hit(shadow_ray, interval(0.001, dist_luz), shadow_rec)) {
                return ambiente; // Retorna SÓ o ambiente, bloqueia difusa e especular
            }

            // Se chegou até aqui, não tem sombra bloqueando a luz
            return ambiente + difusa + especular;
        }

        // Se não bateu em nada, retorna a cor do fundo (preto)
        return color(0, 0, 0);
    }
};

#endif