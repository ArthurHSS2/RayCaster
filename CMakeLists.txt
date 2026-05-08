cmake_minimum_required(VERSION 3.10)

project(RayTracer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG   "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(${PROJECT_NAME}
    main.cpp
    Proc_graf/Core/Camera.cpp
    Proc_graf/Core/Scene.cpp
    Proc_graf/Core/Ray.cpp
    Proc_graf/Core/Render.cpp
    Proc_graf/Geometry/Sphere.cpp
    Proc_graf/Geometry/Plane.cpp
    Proc_graf/Geometry/Mesh.cpp
    Proc_graf/Math/Point3d.cpp
    Proc_graf/Math/Vector3d.cpp
    ../utils/Scene/sceneParser.cpp
    ../utils/MeshReader/ObjReader.cpp
)

# diz ao compilador onde procurar headers
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}
)