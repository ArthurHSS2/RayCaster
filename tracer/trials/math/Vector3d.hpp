#include <cmath>
#include <iostream>

#ifndef VECTOR3D_H

#define VECTOR3D_H

class Vector3d
{
    public:
        double x, y, z;

		Vector3d(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

        double produto_escalar(const Vector3d& outro) const 
        {
            return x*outro.x + y*outro.y + z*outro.z;
        }

        Vector3d produto_vetorial(const Vector3d outro) const 
        {
            return Vector3d(y*outro.z - outro.y*z, outro.x*z - x*outro.z, x*outro.y - outro.x*y);
        }

        Vector3d operator+ (const Vector3d&v) const
        {
            return Vector3d(x+v.x, y+v.y, z+v.z);
        }

        Vector3d operator- (const Vector3d&v) const 
        {
            return Vector3d(x-v.x, y-v.y, z-v.z);
        }

        Vector3d operator* (const double escalar) const 
        {
            return Vector3d(x*escalar, y*escalar, z*escalar);
        }

        Vector3d operator/ (const double escalar) const 
        {
            if (escalar == 0) return Vector3d(0,0,0);
            return Vector3d(x/escalar, y/escalar, z/escalar);
        }

        double modulo() const 
        {
            return sqrt(x*x + y*y + z*z);
        }

        double modulo_quadrado() const 
        {
            return (x*x + y*y + z*z);
        }

        Vector3d normalizacao() const 
        { 
            double mod = (*this).modulo();
            return Vector3d(x/mod, y/mod, z/mod);
        }

        Vector3d operator-() const {
            return Vector3d(-x, -y, -z);
        }

        double getX() const { return x; }
        double getY() const { return y; }
        double getZ() const { return z; }
};

using Point3d = Vector3d;

#endif
