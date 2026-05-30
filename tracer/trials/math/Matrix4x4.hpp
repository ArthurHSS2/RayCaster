#ifndef MATRIX4X4_H
#define MATRIX4X4_H

#include "Vector3d.hpp" // Apenas o ficheiro unificado
#include <array>

class Matrix4x4 {
public:
    std::array<std::array<double, 4>, 4> m = {{
        {1.0, 0.0, 0.0, 0.0},
        {0.0, 1.0, 0.0, 0.0},
        {0.0, 0.0, 1.0, 0.0},
        {0.0, 0.0, 0.0, 1.0}
    }};

    Matrix4x4() = default;

    static Matrix4x4 translation(double dx, double dy, double dz);
    static Matrix4x4 scaling(double sx, double sy, double sz);
    static Matrix4x4 rotationX(double angle);
    static Matrix4x4 rotationY(double angle);
    static Matrix4x4 rotationZ(double angle);

    // Multiplicação de Matriz x Matriz continua com o operador *
    Matrix4x4 operator*(const Matrix4x4& other) const;

    // Funções explícitas em vez de operadores
    Vector3d multiply_point(const Vector3d& p) const;
    Vector3d multiply_vector(const Vector3d& v) const;
};

#endif