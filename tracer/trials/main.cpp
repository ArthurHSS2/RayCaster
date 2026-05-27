#include "color.hpp"
#include "commom.hpp"
#include "hittable.h"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "camera.hpp"
#include "light.hpp"
#include <memory>
#include <vector>

int main() {
    hittable_list world;

    // MATERIAL DA ESFERA (Vermelho, estilo plástico brilhante)
    color esfera_ka(0.1, 0.0, 0.0); // Ambiente baixo
    color esfera_kd(1.0, 0.0, 0.0); // Difuso vermelho puro
    color esfera_ks(1.0, 1.0, 1.0); // Especular branco (brilho)
    double esfera_ns = 32.0;        // Foco do brilho bem concentrado

    world.add(std::make_shared<sphere>(Point3d(0, 0, -1), 0.5, esfera_ka, esfera_kd, esfera_ks, esfera_ns));

    // MATERIAL DO PLANO (Chão verde fosco)
    color plano_ka(0.0, 0.1, 0.0);
    color plano_kd(0.0, 0.8, 0.0);
    color plano_ks(0.1, 0.1, 0.1); // Especular bem baixo para o chão não parecer de vidro
    double plano_ns = 10.0;        // Brilho mais espalhado

    world.add(std::make_shared<plane>(Point3d(0, -0.5, 0), Vector3d(0, 1, 0), plano_ka, plano_kd, plano_ks, plano_ns));


    // CONFIGURAÇÃO DE ILUMINAÇÃO (usando um vetor para múltiplas luzes)
    std::vector<PointLight> luzes_da_cena;

    // Lâmpada 1: posicionada no alto (Y=5), para a direita (X=5) e um pouco para frente do cenário (Z=-2)
    luzes_da_cena.push_back(PointLight(Point3d(5.0, 5.0, -2.0), color(1.0, 1.0, 1.0)));
    
    // Lâmpada 2: luz de preenchimento azul do lado esquerdo para testar a soma na esfera
    luzes_da_cena.push_back(PointLight(Point3d(-5.0, 2.0, -2.0), color(0.0, 0.0, 0.5)));
    
    // Luz ambiente suave para garantir que as áreas de sombra não fiquem 100% pretas
    color luz_ambiente_global(0.2, 0.2, 0.2); 


    // CONFIGURAÇÃO DA CÂMERA
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 400;

    // O render recebe o vetor de luzes completo
    cam.render(world, luzes_da_cena, luz_ambiente_global);

    return 0;
}