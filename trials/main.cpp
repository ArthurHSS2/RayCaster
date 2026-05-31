#include "math/Matrix4x4.hpp"
#include "color.hpp"
#include "commom.hpp"
#include "hittable.h"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "camera.hpp"
#include "light.hpp"

#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <cmath> // Adicionado para reconhecer o M_PI

// Incluindo o parser e o leitor de OBJ do repositório
#include "utils/Scene/sceneParser.cpp"
#include "utils/MeshReader/ObjReader.cpp"

int main() {
    try {
        // CARREGAR A CENA
        std::string arquivo_cena = "trials/utils/input/caso3.json";
        std::cerr << "A carregar a cena: " << arquivo_cena << "...\n";
        
        SceneData scene = SceneJsonLoader::loadFile(arquivo_cena);
        
        hittable_list world;
        std::vector<PointLight> luzes_da_cena;

        // CONFIGURAR LUZES
        for (auto& light : scene.lightList) {
            Point3d pos(light.pos.getX(), light.pos.getY(), light.pos.getZ());
            color intensidade(light.color.r, light.color.g, light.color.b);
            luzes_da_cena.push_back(PointLight(pos, intensidade));
        }

        // Garante luz se o JSON não tiver
        if (luzes_da_cena.empty()) {
            std::cerr << "Aviso: Nenhuma luz encontrada. Injetando Luz de Emergencia...\n";
            Point3d posLuz(scene.camera.lookfrom.getX(), scene.camera.lookfrom.getY() + 10, scene.camera.lookfrom.getZ());
            luzes_da_cena.push_back(PointLight(posLuz, color(1.0, 1.0, 1.0)));
        }

        color luz_ambiente_global(scene.globalLight.color.r, scene.globalLight.color.g, scene.globalLight.color.b);

        // PROCESSAR OBJETOS E MALHAS
        for (auto& obj : scene.objects) {
            
            // Extrair materiais comuns a todos os objetos
            color ka(obj.material.ka.r, obj.material.ka.g, obj.material.ka.b);
            color kd(obj.material.color.r, obj.material.color.g, obj.material.color.b);
            color ks(obj.material.ks.r, obj.material.ks.g, obj.material.ks.b);
            double ns = obj.material.ns;

            // PROCESSAR TRANSFORMAÇÕES COM SEGURANÇA
            Matrix4x4 matFinal; 
            double detEscala = 1.0; // Rastreia se o objeto foi virado do avesso
            bool usou_translacao_no_transform = false; // 💡 Rastreador de Dupla Translação

            for (auto& t : obj.transforms) {
                Matrix4x4 step;
                if (t.tType == "translation") {
                    step = Matrix4x4::translation(t.data.getX(), t.data.getY(), t.data.getZ());
                    usou_translacao_no_transform = true; // Avisa que já movemos o objeto!
                }
                else if (t.tType == "scaling") {
                    step = Matrix4x4::scaling(t.data.getX(), t.data.getY(), t.data.getZ());
                    detEscala *= (t.data.getX() * t.data.getY() * t.data.getZ());
                }
                else if (t.tType == "rotation") {
                    // Converter graus para radianos!
                    double radX = t.data.getX() * M_PI / 180.0;
                    double radY = t.data.getY() * M_PI / 180.0;
                    double radZ = t.data.getZ() * M_PI / 180.0;
                    step = Matrix4x4::rotationZ(radZ) * Matrix4x4::rotationY(radY) * Matrix4x4::rotationX(radX);
                }
                matFinal = step * matFinal;
            }

            // (Evita dupla translação)Aplica apenas se o JSON não tiver usado "transform" de translação
            if (!usou_translacao_no_transform) {
                matFinal = Matrix4x4::translation(obj.relativePos.getX(), obj.relativePos.getY(), obj.relativePos.getZ()) * matFinal;
            }

            // Instanciar consoante o tipo geométrico
            if (obj.objType == "sphere") {
                double radius = obj.numericData["radius"];
                Point3d center = matFinal.multiply_point(Point3d(0, 0, 0)); 
                world.add(std::make_shared<sphere>(center, radius, ka, kd, ks, ns));
            }
            else if (obj.objType == "plane") {
                Vetor normJSON = obj.vetorPointData["normal"];
                Vector3d normal = matFinal.multiply_vector(Vector3d(normJSON.getX(), normJSON.getY(), normJSON.getZ())).normalizacao();
                
                // Lendo o point_on_plane em vez de assumir (0,0,0)
                Vetor ptJSON = obj.vetorPointData["point_on_plane"];
                Point3d point = matFinal.multiply_point(Point3d(ptJSON.getX(), ptJSON.getY(), ptJSON.getZ()));
                
                world.add(std::make_shared<plane>(point, normal, ka, kd, ks, ns));
            }
            else if (obj.objType == "mesh") {
                std::string path = obj.otherProperties["path"];
                if (path.empty()) path = obj.otherProperties["file"];
                
                // Força a encontrar o arquivo na pasta correta
                std::string nome_arquivo = path.substr(path.find_last_of("/\\") + 1);
                path = "trials/utils/input/" + nome_arquivo;

                std::cerr << "Lendo malha: " << path << "\n";

                objReader reader(path);
                auto faces = reader.getFacePoints(); 

                // Se o determinante for negativo, a normal precisa ser espelhada
                bool invertNormals = (detEscala < 0.0);

                // 2. TRIANGULAÇÃO DE QUADS E N-GONS (Fan Triangulation)
                for(auto& face : faces) {
                    if(face.size() >= 3) { 
                        // Fatiar qualquer polígono em triângulos sequenciais
                        for (size_t i = 1; i < face.size() - 1; ++i) {
                            Point3d vA = matFinal.multiply_point(Point3d(face[0].getX(), face[0].getY(), face[0].getZ()));
                            Point3d vB = matFinal.multiply_point(Point3d(face[i].getX(), face[i].getY(), face[i].getZ()));
                            Point3d vC = matFinal.multiply_point(Point3d(face[i+1].getX(), face[i+1].getY(), face[i+1].getZ()));

                            // Calcula a normal da face transformada
                            Vector3d edge1 = vB - vA;
                            Vector3d edge2 = vC - vA;
                            Vector3d flat_normal = edge1.produto_vetorial(edge2).normalizacao();                            
                            // Conserta a normal caso a malha esteja do avesso
                            if (invertNormals) {
                                flat_normal = flat_normal * -1.0;
                            }

                            world.add(std::make_shared<triangle>(vA, vB, vC, flat_normal, flat_normal, flat_normal, ka, kd, ks, ns));
                        }
                    }
                }
            }
        }

        // CONFIGURAR A CÂMARA
        camera cam;
        
        // Reduz resoluções absurdas
        double max_width = 600.0;
        if (scene.camera.image_width > max_width) {
            cam.image_width = max_width;
            cam.image_height = max_width / ((double)scene.camera.image_width / (double)scene.camera.image_height);
            std::cerr << "Resolucao otimizada para " << cam.image_width << "x" << cam.image_height << "!\n";
        } else {
            cam.image_width  = scene.camera.image_width;
            cam.image_height = scene.camera.image_height;
        }

        cam.aspect_ratio = (double)cam.image_width / (double)cam.image_height;
        cam.vfov = 90.0; // Mantemos o FOV fixo até haver um cálculo de FOV a partir do screen_distance
        
        cam.lookfrom = Point3d(scene.camera.lookfrom.getX(), scene.camera.lookfrom.getY(), scene.camera.lookfrom.getZ());
        cam.lookat   = Point3d(scene.camera.lookat.getX(), scene.camera.lookat.getY(), scene.camera.lookat.getZ());
        cam.vup      = Vector3d(scene.camera.upVector.getX(), scene.camera.upVector.getY(), scene.camera.upVector.getZ());

        // INICIAR RENDER
        std::cerr << "A iniciar o Ray Tracing...\n";
        cam.render(world, luzes_da_cena, luz_ambiente_global);

    } catch (const std::exception& e) {
        std::cerr << "\nErro ao carregar a cena: " << e.what() << "\n";
        return 1;
    }

    return 0;
}