#include "Vector3d.hpp"

#ifndef POINT3D_H

#define POINT3D_H

class Point3d 
{
	public:
		double x, y, z;

		Point3d(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
		
		double distancia_pontos(const Point3d& ponto)const;

		double distancia_quadrada(const Point3d& ponto)const;
		
		Vector3d diferenca_pontos(const Point3d& ponto)const;

        Point3d operator+ (const Vector3d&v)const;
        
        Point3d operator* (const double&k)const;

        Vector3d operator- (const Point3d&p)const;
};

#endif