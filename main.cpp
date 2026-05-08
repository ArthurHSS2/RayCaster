#include <iostream>
#include "src/Ponto.h"
#include "src/Vetor.h"
#include "Proc_graf/Core/Render.hpp"

int main() {
    SceneData sceneparser = SceneJsonLoader::loadFile("utils/input/caso5.json");
    Scene scene(sceneparser);
    Render render;

    Point3d lookfrom = Point3d(sceneparser.camera.lookfrom.getX(),
                                sceneparser.camera.lookfrom.getY(),
                                sceneparser.camera.lookfrom.getZ());
                                
    Point3d lookat = Point3d(sceneparser.camera.lookat.getX(),
                            sceneparser.camera.lookat.getY(),
                            sceneparser.camera.lookat.getZ());
    
    Vector3d up = Vector3d(sceneparser.camera.upVector.getX(),
                            sceneparser.camera.upVector.getY(),
                            sceneparser.camera.upVector.getZ());                             

    Camera cam(lookfrom, lookat, up, sceneparser.camera.screen_distance, sceneparser.camera.image_height, sceneparser.camera.image_width);

    render.renderize(cam, scene);

    return 0;
}