#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>

class Shape;
struct SceneObject;
struct ColorData;
struct SceneData;

class Scene {
private:
    std::unique_ptr<Shape> create_sphere(const SceneObject& obj, const ColorData& color);
    std::unique_ptr<Shape> create_plane(const SceneObject& obj, const ColorData& color);
    std::unique_ptr<Shape> create_mesh(const SceneObject& obj, const ColorData& color);
    std::vector<std::unique_ptr<Shape>> get_shapes(const SceneData& scene);
    
public:
    std::vector<std::unique_ptr<Shape>> Shapes;
    SceneData scene;
    
    Scene(SceneData scene);
};
