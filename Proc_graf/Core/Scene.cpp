// Scene.cpp
#include "Scene.hpp"

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

    std::vector<std::array<Point3d, 3>> lista;
    std::vector<Vector3d> normais_faces;
    std::vector<Point3d> vertices;

    for (const auto& triangle : facePoints) {
        if (triangle.size() == 3) {
            std::array<Point3d, 3> tri;
            for (int i = 0; i < 3; i++) {
                // Convert Ponto to point3d
                tri[i] = Point3d(triangle[i].getX(), 
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


/*std::unique_ptr<Shape> Scene::create_mesh(ObjectData& obj, const ColorData& color) {
    objReader mesh(obj.getProperty("path"));////////////////////////////////////

    std::vector<vector<Ponto>> list = mesh.getFacePoints();
    std::vector<Vetor> norm = mesh.getNormals();
    std::vector<Ponto> vert = mesh.getVertices();

    std::vector<Point3d> vertices;
    std::vector<Vector3d> normais;
    std::vector<std::array<Point3d, 3>> listas;

    vertices.reserve(vert.size()); 
    for (const auto& p : vert) {
        vertices.push_back(Point3d(p.getX(), p.getY(), p.getZ()));
    }

    // 2. Translate Normals (Vetor -> Vector3d)
    normais.reserve(norm.size());
    for (const auto& v : norm) {
        normais.push_back(Vector3d(v.getX(), v.getY(), v.getZ()));
    }

    listas.reserve(list.size());
    for (const auto& face : list) {
        if (face.size() >= 3) { 
            listas.push_back({
                Point3d(face[0].getX(), face[0].getY(), face[0].getZ()),
                Point3d(face[1].getX(), face[1].getY(), face[1].getZ()),
                Point3d(face[2].getX(), face[2].getY(), face[2].getZ())
            });
        }
    }

    return std::make_unique<Mesh>(  
        vertices, listas, normais,
        color.r * 255, color.g * 255, color.b * 255
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
            ObjectData obj1 = obj;///////////////////////////////
            auto mesh = create_mesh(obj1, color);
            if (mesh) {
                shapes.push_back(std::move(mesh));
            }
        }
    }
    
    return shapes;
}
*/