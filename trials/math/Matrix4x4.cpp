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

Vector3d Matrix4x4::multiply_point(const Vector3d& p) const {
    // Para pontos, o W = 1 imaginário permite que a coluna de translação (índice 3) some os valores
    double x = m[0][0]*p.x + m[0][1]*p.y + m[0][2]*p.z + m[0][3]*1.0;
    double y = m[1][0]*p.x + m[1][1]*p.y + m[1][2]*p.z + m[1][3]*1.0;
    double z = m[2][0]*p.x + m[2][1]*p.y + m[2][2]*p.z + m[2][3]*1.0;
    return Vector3d(x, y, z);
}

Vector3d Matrix4x4::multiply_vector(const Vector3d& v) const {
    // Para vetores/normais, o W = 0 anula a coluna de translação! Eles apenas giram e escalam.
    double x = m[0][0]*v.x + m[0][1]*v.y + m[0][2]*v.z;
    double y = m[1][0]*v.x + m[1][1]*v.y + m[1][2]*v.z;
    double z = m[2][0]*v.x + m[2][1]*v.y + m[2][2]*v.z;
    return Vector3d(x, y, z);
}