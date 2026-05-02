// Scene.cpp
#include "Scene.hpp"

point3d convertToPoint3d(const Ponto& p) {
    return point3d(p.getX(), p.getY(), p.getZ());
}

Scene::Scene(SceneData scene) : scene(std::move(scene)) {
    shapes = get_shapes(this->scene);
}


std::unique_ptr<Shape> Scene::create_sphere(const ObjectData& obj, const ColorData& color) {
    Vetor centerVec = obj.vetorPointData.at("center");
    Point3d center(centerVec.getX(), centerVec.getY(), centerVec.getZ());
    double radius = obj.numericData.at("radius");
    
    return std::make_unique<Sphere>(  
        center, radius,
        color.r * 255, color.g * 255, color.b * 255
    );
}

std::unique_ptr<Shape> Scene::create_plane(const ObjectData& obj, const ColorData& color) {
    Vetor pointVec = obj.vetorPointData.at("point_on_plane");
    Vetor normalVec = obj.vetorPointData.at("normal");
    Point3d point(pointVec.getX(), pointVec.getY(), pointVec.getZ());
    Vector3d normal(normalVec.getX(), normalVec.getY(), normalVec.getZ());
    
    return std::make_unique<Plane>(  
        point, normal,
        color.r * 255, color.g * 255, color.b * 255
    );
}

std::unique_ptr<Shape> Scene::create_mesh(const ObjectData& obj, const ColorData& color) {
    auto pathIt = obj.otherProperties.find("path");
    if (pathIt == obj.otherProperties.end()) {
        return nullptr;
    }
    std::string objPath = pathIt->second;
    objReader reader(objPath);

    std::vector<std::vector<Ponto>> facePoints = reader.getFacePoints();

    std::vector<std::array<point3d, 3>> lista;
    std::vector<double> normais_faces;
    std::vector<Point3d> vertices;

    for (const auto& triangle : facePoints) {
        if (triangle.size() == 3) {
            std::array<point3d, 3> tri;
            for (int i = 0; i < 3; i++) {
                // Convert Ponto to point3d
                tri[i] = point3d(triangle[i].getX(), 
                                triangle[i].getY(), 
                                triangle[i].getZ());
            }
            lista.push_back(tri);
        }
    }
    
    return std::make_unique<Mesh>(
        vertices, lista, normais_faces, color.r * 255, color.g * 255, color.b * 255
    );
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