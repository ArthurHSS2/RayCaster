#ifndef MESH_H
#define MESH_H

#include "../Geometry/Shape.hpp"
#include <array>
#include <vector>

class Mesh : public Shape {
    std::vector<Point3d> vertices;
    std::vector<std::array<Point3d, 3>> lista;
    std::vector<Vector3d> normais_faces;


public:
    float R, G, B;

    Mesh(std::vector<Point3d>& vertices, std::vector<std::array<Point3d, 3>>& lista, std::vector<Vector3d> normais_faces, 
        float R, float G, float B) 
    : vertices(vertices), lista(lista), normais_faces(normais_faces), R(R), G(G), B(B) {
        material.R = R; material.G = G; material.B = B;
    }

    double intersect(const Ray& ray) const override;

    Vector3d get_normal(const Point3d& p) const override;

    std::string get_type() const override {return "Mesh";}
};

#endif
