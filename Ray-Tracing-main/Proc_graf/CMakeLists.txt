cmake_minimum_required(VERSION 3.10)

project(RayTracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG   "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(${PROJECT_NAME}
    main.cpp
    Core/Camera.cpp
    Core/Scene.cpp
    Core/Ray.cpp
    Core/Render.cpp
    Geometry/Sphere.cpp
    Geometry/Plane.cpp
    Math/Point3d.cpp
    Math/Vector3d.cpp
    ../utils/Scene/sceneParser.cpp
    ../utils/MeshReader/ObjReader.cpp
)

# diz ao compilador onde procurar headers
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}
)