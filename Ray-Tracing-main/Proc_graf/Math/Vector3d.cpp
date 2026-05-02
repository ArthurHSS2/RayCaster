#include "Vector3d.hpp"

double Vector3d::produto_escalar(const Vector3d& outro) const 
{
    return x*outro.x + y*outro.y + z*outro.z;
}

Vector3d Vector3d::produto_vetorial(const Vector3d outro) const 
{
    return Vector3d(y*outro.z - outro.y*z, outro.x*z - x*outro.z, x*outro.y - outro.x*y);
}

Vector3d Vector3d::operator+ (const Vector3d&v) const
{
    return Vector3d(x+v.x, y+v.y, z+v.z);
}

Vector3d Vector3d::operator- (const Vector3d&v) const 
{
    return Vector3d(x-v.x, y-v.y, z-v.z);
}

Vector3d Vector3d::operator* (const double escalar) const 
{
    return Vector3d(x*escalar, y*escalar, z*escalar);
}

Vector3d Vector3d::operator/ (const double escalar) const 
{
    if (escalar == 0) return Vector3d(0,0,0);
    return Vector3d(x/escalar, y/escalar, z/escalar);
}

double Vector3d::modulo() const 
{
    return sqrt(x*x + y*y + z*z);
}

Vector3d Vector3d::normalizacao() const 
{ 
    double mod = (*this).modulo();
    return Vector3d(x/mod, y/mod, z/mod);
}