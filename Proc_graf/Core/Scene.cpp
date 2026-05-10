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
    Point3d min_pt(9999999, 9999999, 9999999);
    Point3d max_pt(-9999999, -9999999, -9999999);

    auto pathIt = obj.otherProperties.find("path");
    if (pathIt == obj.otherProperties.end()) {
        return nullptr;
    }

    std::vector<TransformData> transforms = obj.transforms;

    std::string objPath = pathIt->second;
    objReader reader(objPath);

    std::vector<std::vector<Ponto>> facePoints = reader.getFacePoints();

    std::vector<std::array<Point3d, 3>> lista;
    std::vector<Vector3d> normais_faces;
    std::vector<Point3d> vertices;

    // vendo tranformações
    Matrix4x4 matrizAcumulada;

    for(const auto& trans : transforms) {
        Matrix4x4 transformacaoAtual;

        if (trans.tType == "translation"){
            Vector3d t = Vector3d(trans.data.getX(), trans.data.getY(), trans.data.getZ());
            // Chama a função estática diretamente da Classe
            transformacaoAtual = Matrix4x4::translation(t.x, t.y, t.z);
        }
        else if (trans.tType == "scaling"){
            Vector3d s = Vector3d(trans.data.getX(), trans.data.getY(), trans.data.getZ());
            transformacaoAtual = Matrix4x4::scaling(s.x, s.y, s.z);
        }
        else if (trans.tType == "rotation"){
            Vector3d r = Vector3d(trans.data.getX(), trans.data.getY(), trans.data.getZ());
            // Combina as rotações e guarda em transformacaoAtual
            transformacaoAtual = Matrix4x4::rotationZ(r.z) * Matrix4x4::rotationY(r.y) * Matrix4x4::rotationX(r.x);
        }
        matrizAcumulada = transformacaoAtual * matrizAcumulada;
    }

    for (const auto& triangle : facePoints) {
        if (triangle.size() == 3) {
            std::array<Point3d, 3> tri;
            for (int i = 0; i < 3; i++) {
                // Convert Ponto to point3d
                if (triangle[i].getX() < min_pt.x) min_pt.x = triangle[i].getX();
                if (triangle[i].getY() < min_pt.y) min_pt.y = triangle[i].getY();
                if (triangle[i].getZ() < min_pt.z) min_pt.z = triangle[i].getZ();
                
                if (triangle[i].getX() > max_pt.x) max_pt.x = triangle[i].getX();
                if (triangle[i].getY() > max_pt.y) max_pt.y = triangle[i].getY();
                if (triangle[i].getZ() > max_pt.z) max_pt.z = triangle[i].getZ();

                tri[i] = Point3d(triangle[i].getX(), 
                                 triangle[i].getY(), 
                                 triangle[i].getZ());
            }
            lista.push_back(tri);
        }
    }

    Point3d centro(
        (min_pt.x + max_pt.x) / 2.0,
        (min_pt.y + max_pt.y) / 2.0,
        (min_pt.z + max_pt.z) / 2.0
    );

    Matrix4x4 T_ida = Matrix4x4::translation(-centro.x, -centro.y, -centro.z);

    Matrix4x4 T_volta = Matrix4x4::translation(centro.x, centro.y, centro.z);

    matrizAcumulada = T_volta*matrizAcumulada*T_ida;

    for (auto& triangle : lista) {
        if (triangle.size() == 3) {
            for (int i = 0; i < 3; i++) {
                triangle[i] = matrizAcumulada*triangle[i];
            }
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