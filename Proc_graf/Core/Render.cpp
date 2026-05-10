#include "Render.hpp"
#include <filesystem>

void Render::renderize(const Camera& cam, const Scene& cena) {
    std::ofstream file("out.ppm");
    std::cout << std::filesystem::current_path() << std::endl;
    file << "P3\n" << cam.width << " " << cam.height << "\n255\n";
    
    // Render loop
    for (int i = 0; i < cam.height; i++) {
        for (int j = 0; j < cam.width; j++) {
            Ray ray = cam.generate_ray(i, j);
            
            double closest_t = -1.0;
            int r = 0, g = 0, b = 0;
            
            // Check spheres
            for (const auto& shape : cena.shapes) {
                    double t = shape->intersect(ray);
                    if (t > 0 && (closest_t < 0 || t < closest_t)) {
                        closest_t = t;
                        r = shape->material.R;
                        g = shape->material.G;
                        b = shape->material.B;
                    }
            }
            
            if (closest_t > 0) {
                file << r << " " << g << " " << b << " ";
            } else {
                file << "0 0 0 ";
            }
            file << "\n";
        }
        
    }
    file.close();
}