#include "Camera.hpp"

Camera::Camera(Point3d pos, Point3d mira, Vector3d Up, double dist, int height, int width) : C(pos), M(mira), Up(Up), dist(dist), height(height), width(width)
{
    aspect_ratio = (double)width / height; 
    direcao = M-C;
    W = direcao.normalizacao()*(-1);
    U = Up.produto_vetorial(W).normalizacao();
    V = W.produto_vetorial(U);
}

Ray Camera::generate_ray(int row, int col) const 
{
    double u = ((col + 0.5f) / width - 0.5f) * aspect_ratio;
    double v = 0.5f - (row + 0.5f) / height;
    Vector3d dir = U * u + V * v - W * dist;
    return Ray(C, dir);
}