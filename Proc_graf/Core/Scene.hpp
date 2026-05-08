#ifndef SCENE_H
#define SCENE_H

#include "../Geometry/Shape.hpp"
#include "../Geometry/Sphere.hpp"
#include "../Geometry/Plane.hpp"
#include "../Geometry/Mesh.hpp"
#include "../../utils/Scene/sceneParser.cpp"
#include "../../utils/MeshReader/ObjReader.cpp"
#include <vector>
#include <memory>

class Scene {
private:
    std::unique_ptr<Shape> create_sphere(const ObjectData& obj, const ColorData& color);
    std::unique_ptr<Shape> create_plane(const ObjectData& obj, const ColorData& color);
    std::unique_ptr<Shape> create_mesh(const ObjectData& obj, const ColorData& color);//////////////////
    std::vector<std::unique_ptr<Shape>> get_shapes(const SceneData& scene);
    
public:
    std::vector<std::unique_ptr<Shape>> shapes;
    SceneData scene;
    
    Scene(SceneData scene);
};

#endif