#include <iostream>
#include <cmath>
class Vector3d{
	public:
		float x, y, z;
		
		Vector3d(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

        float produto_escalar(const Vector3d& outro)const {
		return x*outro.x + y*outro.y + z*outro.z;
	}

        Vector3d produto_vetorial(const Vector3d outro)const {
		return Vector3d(y*outro.z - outro.y*z, outro.x*z - x*outro.z, x*outro.y - outro.x*y);
	}

        Vector3d operator+ (const Vector3d&v) {
            return Vector3d(x+v.x, y+v.y, z+v.z);
        }

        Vector3d operator- (const Vector3d&v) {
            return Vector3d(x-v.x, y-v.y, z-v.z);
        }

        Vector3d operator* (float escalar) {
            return Vector3d(x*escalar, y*escalar, z*escalar);
        }

        Vector3d operator/ (float escalar) {
            if (escalar == 0) return Vector3d(0,0,0);
            return Vector3d(x/escalar, y/escalar, z/escalar);
        }

        float modulo() {
            return sqrt(x*x + y*y + z*z);
        }

        Vector3d normalizacao() { 
            float mod = (*this).modulo(); //ver se (*this) funciona
            return Vector3d(x/mod, y/mod, z/mod);
        }
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

		float distancia_quadrada(const Ponto3d& ponto)const{
			float dx = ponto.x -x; 
			float dy = ponto.y-y; 
			float dz = ponto.z-z;

			return (dx*dx+dy*dy+dz*dz);
		}
		
		Vector3d diferenca_pontos(const Ponto3d& ponto)const{
			float dx = ponto.x - x;
			float dy = ponto.y - y;
			float dz = ponto.z - z;
			
			return Vector3d(dx,dy,dz);
		}

        Ponto3d operator+ (const Vector3d&v) { 
            return Ponto3d(x+v.x, y+v.y, z+v.z); // soma ponto + vetor
        } 

        Ponto3d operator* (const float&k) {
            return Ponto3d(x*k, y*k, z*k); // produto escalar
        }

        Vector3d operator- (const Ponto3d&p) {
            return Vector3d(x-p.x, y-p.y, z-p.z); // diferenca ponto - ponto
        }

};

class Camera {
	public:
        Ponto3d C, M;
        Vector3d U, V, W, Up;
        float dist;
        int h_res, w_res;
};

int main() {

	return 0;
}
