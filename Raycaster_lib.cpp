#include <iostream>
#include <cmath>
class Vector3d{
	public:
		float x, y, z;
		
		Vector3d(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

};
class Ponto3d {
	public:
		float x, y, z;
		
		Ponto3d(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
		
		float distancia_pontos(const Ponto3d& ponto)const{
			float dx = ponto.x - x;
			float dy = ponto.y - y;
			float dz = ponto.z - z;
			
			return sqrt(dx*dx + dy*dy + dz*dz); 
		}
		
		Vector3d diferenca_pontos(const Ponto3d& ponto)const{
			float dx = ponto.x - x;
			float dy = ponto.y - y;
			float dz = ponto.z - z;
			
			return Vector3d(dx,dy,dz);
		}
		
};

class Camera {
			
}

int main() {
	
	return 0;
}