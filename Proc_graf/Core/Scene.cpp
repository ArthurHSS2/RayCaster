// Scene.cpp
#include "Scene.hpp"
#include "Shape.hpp"
#include "Sphere.hpp"
#include "Plane.hpp"
#include "Vector3d.hpp"
#include "Point3d.hpp"


Scene::Scene(SceneData scene) : scene(std::move(scene)) {
    Shapes = get_shapes(this->scene);
}


std::unique_ptr<Shape> Scene::create_sphere(const SceneObject& obj, const ColorData& color) {
    Vetor centerVec = obj.vetorPointData.at("center");
    Ponto3d center(centerVec.getX(), centerVec.getY(), centerVec.getZ());
    double radius = obj.numericData.at("radius");
    
    return std::make_unique<Sphere>(  
        center, radius,
        color.r * 255, color.g * 255, color.b * 255
    );
}

std::unique_ptr<Shape> Scene::create_plane(const SceneObject& obj, const ColorData& color) {
    Vetor pointVec = obj.vetorPointData.at("point_on_plane");
    Vetor normalVec = obj.vetorPointData.at("normal");
    Ponto3d point(pointVec.getX(), pointVec.getY(), pointVec.getZ());
    Vetor normal(normalVec.getX(), normalVec.getY(), normalVec.getZ());
    
    return std::make_unique<Plane>(  
        point, normal,
        color.r * 255, color.g * 255, color.b * 255
    );
}

std::unique_ptr<Shape> Scene::create_mesh(const SceneObject& obj, const ColorData& color) {
    
    
    return nullptr;
}

std::vector<std::unique_ptr<Shape>> Scene::get_shapes(const SceneData& scene) {
    std::vector<std::unique_ptr<Shape>> shapes;
    
    for (const auto& obj : scene.objects) {
        ColorData color = obj.material.color;
        
        if (obj.objType == "sphere") {
            shapes.push_back(create_sphere(obj, color));
        }
        else if (obj.objType == "plane") {
            shapes.push_back(create_plane(obj, color));
        }
        else if (obj.objType == "mesh") {
            auto mesh = create_mesh(obj, color);
            if (mesh) {
                shapes.push_back(std::move(mesh));
            }
        }
    }
    
    return shapes;
}
