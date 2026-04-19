#include <iostream>
#include <cmath>

#define HEIGHT_RES 120
#define WIDTH_RES 80

class Vector3d{
	public:
		float x, y, z;
		
		Vector3d(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}

        float produto_escalar(const Vector3d& outro) const {
		return x*outro.x + y*outro.y + z*outro.z;
	}

        Vector3d produto_vetorial(const Vector3d outro) const {
		return Vector3d(y*outro.z - outro.y*z, outro.x*z - x*outro.z, x*outro.y - outro.x*y);
	}

        Vector3d operator+ (const Vector3d&v) const{
            return Vector3d(x+v.x, y+v.y, z+v.z);
        }

        Vector3d operator- (const Vector3d&v) const {
            return Vector3d(x-v.x, y-v.y, z-v.z);
        }

        Vector3d operator* (const float escalar) const {
            return Vector3d(x*escalar, y*escalar, z*escalar);
        }

        Vector3d operator/ (const float escalar) const {
            if (escalar == 0) return Vector3d(0,0,0);
            return Vector3d(x/escalar, y/escalar, z/escalar);
        }

        float modulo() const {
            return sqrt(x*x + y*y + z*z);
        }

        Vector3d normalizacao() const { 
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
		
		Vector3d diferenca_pontos(const Ponto3d& ponto) const{
			float dx = ponto.x - x;
			float dy = ponto.y - y;
			float dz = ponto.z - z;
			
			return Vector3d(dx,dy,dz);
		}

        Ponto3d operator+ (const Vector3d&v) const { 
            return Ponto3d(x+v.x, y+v.y, z+v.z); // soma ponto + vetor
        } 
        


        Ponto3d operator* (const float&k) const {
            return Ponto3d(x*k, y*k, z*k); // produto escalar
        }

        Vector3d operator- (const Ponto3d&p) const {
            return Vector3d(x-p.x, y-p.y, z-p.z); // diferenca ponto - ponto
        }

};

// classe para o raio que sai do olho da camera
class Ray {
public:
    Ponto3d origin;
    Vector3d direction;

    Ray(const Ponto3d& o, const Vector3d& d) : origin(o), direction(d.normalizacao()) {}

    // P(t) = O + t*D -> Onde o raio está na distância t?
    Ponto3d at(float t) const {
        return origin + (direction * t);
    }
};

class Camera {
	public:
        Ponto3d C, M;
        Vector3d U, V, W;
        Vector3d Up = Vector3d(0, 1, 0);
        float dist;
        Vector3d direcao;

        Camera(Ponto3d pos, Ponto3d mira, float dist) : C(pos), M(mira) {
            direcao = M-C;
            W = direcao.normalizacao()*(-1); //Eixo Z
            U = Up.produto_vetorial(W).normalizacao(); //Eixo X
            V = W.produto_vetorial(U); //Eixo Y
        }

        Ray generate_ray(int i, int j) const {
        // Mapeia o pixel para coordenadas -0.5 a 0.5
        float u = (j - WIDTH_RES / 2.0f) / WIDTH_RES;
        float v = (HEIGHT_RES / 2.0f - i) / HEIGHT_RES; // Invertemos o i para o Y crescer para cima

        // Direção: combinação dos eixos U, V e a distância focal em W
        Vector3d dir = (U * u + V * v - W * dist);
        return Ray(C, dir);
    }
};

class Esfera {
public:
    Ponto3d center;
    float   radius;
    float R, G, B; //ajeitar RGB

    Esfera(Ponto3d c, float r) : center(c), radius(r) {}

    // retorna t > 0 se houver interseção, -1 caso contrário
    float intersect(const Ray& ray) const {
        Vector3d oc = ray.origin - center; // Vetor da esfera para a câmera
        
        // Coeficientes da equação at² + bt + c = 0
        float a = ray.direction.produto_escalar(ray.direction);
        float b = 2.0f * oc.produto_escalar(ray.direction);
        float c = oc.produto_escalar(oc) - radius * radius;
        
        float delta = b*b - 4*a*c;

        if (delta < 0) return -1.0f; // Errou a esfera
        
        // Retorna a distância t (ponto de impacto)
        return (-b - std::sqrt(delta)) / (2.0f * a);
    }
};

void renderizar(const Camera& cam, const Esfera& esfera) {
    std::ofstream file("imagem.ppm");
    file << "P3\n" << WIDTH_RES << " " << HEIGHT_RES << "\n255\n";

    for (int i = 0; i < HEIGHT_RES; i++) {
        for (int j = 0; j < WIDTH_RES; j++) {
            // 1. Gera o raio para este pixel
            Ray ray = cam.generate_ray(i, j);
            
            // 2. Testa colisão
            if (esfera.intersect(ray) > 0.0f) {
                file << esfera.r << " " << esfera.g << " " << esfera.b << " ";
            } else {
                file << "0 0 0 "; // Fundo preto
            }
        }
        file << "\n";
    }
    file.close();
}

int main() {
    // Câmera na origem olhando para -Z
    Camera cam(Ponto3d(0,0,0), Ponto3d(0,0,-1), 1.0f);
    
    // Esfera vermelha no centro, recuada 5 unidades
    Esfera bola(Ponto3d(0,0,-5), 2.0f, 255, 0, 0);

    render_to_ppm(cam, bola);
    return 0;
}
