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

// Incluindo o parser e o leitor de OBJ do repositório
#include "utils/Scene/sceneParser.cpp"
#include "utils/MeshReader/ObjReader.cpp"

int main() {
    try {
        // CARREGAR A CENA
        // trocar por "utils/input/monkeyScene.json" ou "mirrorScene.json" depois
        std::string arquivo_cena = "trials/utils/input/monkeyScene.json";
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
        color luz_ambiente_global(scene.globalLight.color.r, scene.globalLight.color.g, scene.globalLight.color.b);

        // PROCESSAR OBJETOS E MALHAS
        for (auto& obj : scene.objects) {
            
            // Extrair materiais comuns a todos os objetos
            color ka(obj.material.ka.r, obj.material.ka.g, obj.material.ka.b);
            color kd(obj.material.color.r, obj.material.color.g, obj.material.color.b);
            color ks(obj.material.ks.r, obj.material.ks.g, obj.material.ks.b);
            double ns = obj.material.ns;

            // Criar a matriz de transformação do objeto lendo as instruções do JSON
            Matrix4x4 matFinal; 
            for (auto& t : obj.transforms) {
                Matrix4x4 step;
                if (t.tType == "translation") step = Matrix4x4::translation(t.data.getX(), t.data.getY(), t.data.getZ());
                else if (t.tType == "scaling") step = Matrix4x4::scaling(t.data.getX(), t.data.getY(), t.data.getZ());
                else if (t.tType == "rotation") step = Matrix4x4::rotationZ(t.data.getZ()) * Matrix4x4::rotationY(t.data.getY()) * Matrix4x4::rotationX(t.data.getX());
                matFinal = step * matFinal;
            }
            // Aplica a posição relativa final
            matFinal = Matrix4x4::translation(obj.relativePos.getX(), obj.relativePos.getY(), obj.relativePos.getZ()) * matFinal;

            // Instanciar consoante o tipo geométrico
            if (obj.objType == "sphere") {
                double radius = obj.numericData["radius"];
                Point3d center = matFinal.multiply_point(Point3d(0, 0, 0)); 
                world.add(std::make_shared<sphere>(center, radius, ka, kd, ks, ns));
            }
            else if (obj.objType == "plane") {
                Vetor normJSON = obj.vetorPointData["normal"];
                Vector3d normal = matFinal.multiply_vector(Vector3d(normJSON.getX(), normJSON.getY(), normJSON.getZ())).normalizacao();
                Point3d point = matFinal.multiply_point(Point3d(0, 0, 0));
                world.add(std::make_shared<plane>(point, normal, ka, kd, ks, ns));
            }
            else if (obj.objType == "mesh") {
                std::string path = obj.otherProperties["path"];
                if (path.empty()) path = obj.otherProperties["file"];
                
                // Garantir que encontra o arquivo na pasta correta se o JSON só passar o nome
                if (path.find("utils/") == 0) {
                    path = "trials/" + path;
                } else if (path.find("/") == std::string::npos) {
                    path = "trials/utils/input/" + path;
                }

                objReader reader(path);
                auto faces = reader.getFacePoints(); 

                // Transforma e gera triângulos para cada face lida
                for(auto& face : faces) {
                    if(face.size() >= 3) { 
                        Point3d vA = matFinal.multiply_point(Point3d(face[0].getX(), face[0].getY(), face[0].getZ()));
                        Point3d vB = matFinal.multiply_point(Point3d(face[1].getX(), face[1].getY(), face[1].getZ()));
                        Point3d vC = matFinal.multiply_point(Point3d(face[2].getX(), face[2].getY(), face[2].getZ()));

                        // Calcula a normal da face transformada
                        Vector3d edge1 = vB - vA;
                        Vector3d edge2 = vC - vA;
                        Vector3d flat_normal = edge1.produto_vetorial(edge2).normalizacao();

                        world.add(std::make_shared<triangle>(vA, vB, vC, flat_normal, flat_normal, flat_normal, ka, kd, ks, ns));
                    }
                }
            }
        }

        // CONFIGURAR A CÂMARA
        camera cam;
        cam.image_width  = scene.camera.image_width;
        cam.aspect_ratio = (double)scene.camera.image_width / (double)scene.camera.image_height;
        cam.vfov = 90.0;
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