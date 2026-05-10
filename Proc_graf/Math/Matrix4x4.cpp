#include "Matrix4x4.hpp"


Matrix4x4 Matrix4x4::translation(double dx, double dy, double dz) {
    Matrix4x4 res; // Cria uma identidade limpa
    res.m[0][3] = dx;
    res.m[1][3] = dy;
    res.m[2][3] = dz;
    return res; // IMPORTANTE: retornar a matriz criada
}

Matrix4x4 Matrix4x4::scaling(double sx, double sy, double sz) {
    Matrix4x4 res;
    res.m[0][0] = sx;
    res.m[1][1] = sy;
    res.m[2][2] = sz;
    return res;
}

Matrix4x4 Matrix4x4::rotationX(double angle) {
    double rad = angle * M_PI / 180.0;
    Matrix4x4 res;
    res.m[1][1] = cos(rad);
    res.m[1][2] = -sin(rad);
    res.m[2][1] = sin(rad);
    res.m[2][2] = cos(rad);
    return res;
}

Matrix4x4 Matrix4x4::rotationY(double angle) {
    double rad = angle * M_PI / 180.0;
    Matrix4x4 res;
    res.m[0][0] = cos(rad);
    res.m[0][2] = sin(rad);
    res.m[2][0] = -sin(rad);
    res.m[2][2] = cos(rad);
    return res;
}

Matrix4x4 Matrix4x4::rotationZ(double angle) {
    double rad = angle * M_PI / 180.0;
    Matrix4x4 res;
    res.m[0][0] = cos(rad);
    res.m[0][1] = -sin(rad);
    res.m[1][0] = sin(rad);
    res.m[1][1] = cos(rad);
    return res;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& other) const {
    Matrix4x4 res;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            double soma = 0;
            for (int k = 0; k < 4; k++) {
                soma += this->m[i][k] * other.m[k][j];
            }
            res.m[i][j] = soma;
        }
    }
    return res;
}

Point3d Matrix4x4::operator*(const Point3d& p) const {
    return Point3d(
        m[0][0]*p.x + m[0][1]*p.y + m[0][2]*p.z + m[0][3],
        m[1][0]*p.x + m[1][1]*p.y + m[1][2]*p.z + m[1][3],
        m[2][0]*p.x + m[2][1]*p.y + m[2][2]*p.z + m[2][3]
    );
}

Vector3d Matrix4x4::operator*(const Vector3d& v) const {
    return Vector3d(
        m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z,
        m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z,
        m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z
    );
}