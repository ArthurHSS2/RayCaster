#include "Point3d.hpp"

double Point3d::distancia_pontos(const Point3d& ponto)const{
    double dx = ponto.x - x; 
    double dy = ponto.y - y; 
    double dz = ponto.z - z;
    
    return sqrt(dx*dx + dy*dy + dz*dz); 
}

double Point3d::distancia_quadrada(const Point3d& ponto)const{
    double dx = ponto.x -x; 
    double dy = ponto.y-y; 
    double dz = ponto.z-z;

    return (dx*dx+dy*dy+dz*dz);
}

Vector3d Point3d::diferenca_pontos(const Point3d& ponto) const{
    double dx = ponto.x - x;
    double dy = ponto.y - y;
    double dz = ponto.z - z;
    
    return Vector3d(dx,dy,dz);
}

Point3d Point3d::operator+ (const Vector3d&v) const { 
    return Point3d(x+v.x, y+v.y, z+v.z); // soma ponto + vetor
} 

Point3d Point3d::operator* (const double&k) const {
    return Point3d(x*k, y*k, z*k); // produto escalar
}

Vector3d Point3d::operator- (const Vector3d&p) const {
    return Vector3d(x-p.x, y-p.y, z-p.z); // diferenca ponto - ponto
}

Vector3d Point3d::operator- (const Point3d&p) const {
    return Vector3d(x-p.x, y-p.y, z-p.z); // diferenca ponto - ponto
}

