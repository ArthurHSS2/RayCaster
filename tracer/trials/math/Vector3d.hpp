#include <cmath>
#include <iostream>

#ifndef VECTOR3D_H

#define VECTOR3D_H

class Point3d;

class Vector3d
{
    public:
        double x, y, z;

		Vector3d(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

        double produto_escalar(const Vector3d& outro)const;

        Vector3d produto_vetorial(const Vector3d outro)const;

        Vector3d operator+ (const Vector3d&v)const;

        Vector3d operator- (const Vector3d&v)const;

        Vector3d operator- (const Point3d&v)const;

        Vector3d operator* (const double escalar)const;

        Vector3d operator/ (const double escalar)const;

        double modulo()const;

        double modulo_quadrado()const;

        Vector3d normalizacao()const;

        Vector3d operator-() const;
};

#endif
