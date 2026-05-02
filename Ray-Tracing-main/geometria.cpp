#include <iostream>
#include <cmath>
#include <fstream>
#include "utils/Scene/sceneParser.cpp"
#include "utils/MeshReader/ObjReader.cpp"

SceneData scene = SceneJsonLoader::loadFile("utils/input/caso5.json");

#define HEIGHT_RES scene.camera.image_height
#define WIDTH_RES scene.camera.image_width

class Vector3d{
	public:
		double x, y, z;
		
		Vector3d(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

        double produto_escalar(const Vector3d& outro) const {
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

        Vector3d operator* (const double escalar) const {
            return Vector3d(x*escalar, y*escalar, z*escalar);
        }

        Vector3d operator/ (const double escalar) const {
            if (escalar == 0) return Vector3d(0,0,0);
            return Vector3d(x/escalar, y/escalar, z/escalar);
        }

        double modulo() const {
            return sqrt(x*x + y*y + z*z);
        }

        Vector3d normalizacao() const { 
            double mod = (*this).modulo(); //ver se (*this) funciona
            return Vector3d(x/mod, y/mod, z/mod);
        }
};

class Ponto3d {
	public:
		double x, y, z;
		
		Ponto3d(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
		
		double distancia_pontos(const Ponto3d& ponto)const{
			double dx = ponto.x - x; 
			double dy = ponto.y - y; 
			double dz = ponto.z - z;
			
			return sqrt(dx*dx + dy*dy + dz*dz); 
		}

		double distancia_quadrada(const Ponto3d& ponto)const{
			double dx = ponto.x -x; 
			double dy = ponto.y-y; 
			double dz = ponto.z-z;

			return (dx*dx+dy*dy+dz*dz);
		}
		
		Vector3d diferenca_pontos(const Ponto3d& ponto) const{
			double dx = ponto.x - x;
			double dy = ponto.y - y;
			double dz = ponto.z - z;
			
			return Vector3d(dx,dy,dz);
		}

        Ponto3d operator+ (const Vector3d&v) const { 
            return Ponto3d(x+v.x, y+v.y, z+v.z); // soma ponto + vetor
        } 
        


        Ponto3d operator* (const double&k) const {
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
        Ponto3d at(double t) const {
            return origin + (direction * t);
        }
};

class Camera {
	public:
        Ponto3d C, M;
        Vector3d U, V, W;
        Vector3d Up;
        double dist;
        Vector3d direcao;
        double aspect_ratio; 

        Camera(Ponto3d pos, Ponto3d mira, Vector3d Up, double dist) : C(pos), M(mira), Up(Up), dist(dist) {
            aspect_ratio = (double)WIDTH_RES / HEIGHT_RES; 
            direcao = M-C;
            W = direcao.normalizacao()*(-1);
            U = Up.produto_vetorial(W).normalizacao();
            V = W.produto_vetorial(U);
        }

        Ray generate_ray(int row, int col) const {
            double u = ((col + 0.5f) / WIDTH_RES - 0.5f) * aspect_ratio;
            double v = 0.5f - (row + 0.5f) / HEIGHT_RES;
            Vector3d dir = U * u + V * v - W * dist;
            return Ray(C, dir);
        }
};

class Esfera {
    public:
        Ponto3d center;
        double   radius;
        double R, G, B; //ajeitar RGB

        Esfera(Ponto3d c, double r, double R, double G, double B) : center(c), radius(r), R(R), G(G), B(B) {}

        // retorna t > 0 se houver interseção, -1 caso contrário
        double intersect(const Ray& ray) const {
            Vector3d oc = ray.origin - center; // Vetor da esfera para a câmera
            
            // Coeficientes da equação at² + bt + c = 0
            double a = ray.direction.produto_escalar(ray.direction);
            double b = 2.0f * oc.produto_escalar(ray.direction);
            double c = oc.produto_escalar(oc) - radius * radius;
            
            double delta = b*b - 4*a*c;

            if (delta < 0) return -1.0f; // Errou a esfera
            
            // Retorna a distância t (ponto de impacto)
            return (-b - std::sqrt(delta)) / (2.0f * a);
        }
};

class Plano {
    public:
        Ponto3d point;
        Vector3d normal;
        int R, G, B;
        
        Plano(Ponto3d p, Vector3d n, int R, int G, int B) 
            : point(p), normal(n.normalizacao()), R(R), G(G), B(B) {}
        
        double intersect(const Ray& ray) const {
            double denom = ray.direction.produto_escalar(normal);
            
            if (std::abs(denom) < 1e-6) return -1.0f; //Epsilon pra evitar problema com 0
            
            Vector3d p0_minus_origin = point - ray.origin;
            double t = p0_minus_origin.produto_escalar(normal) / denom;
            
            if (t < 0.001f) return -1.0f; // Epsilon
            
            return t;
        }
};

class Cena {
    public:
        std::vector<Esfera> esferas;
        std::vector<Plano> planos;
        SceneData scene;
        
        Cena(SceneData scene) : scene(scene) {
            planos = get_planes();
            esferas = get_spheres();
        }

    private:
        std::vector<Esfera> get_spheres() {
            std::vector<Esfera> spheres;
            for (const auto& obj : scene.objects) {
                ColorData color = obj.material.color;
                
                if (obj.objType == "sphere") {
                    Vetor centerVec = obj.vetorPointData.at("center");
                    Ponto3d center(centerVec.getX(), centerVec.getY(), centerVec.getZ());
                    double radius = obj.numericData.at("radius");
                    
                    spheres.push_back(Esfera(center, radius, 
                                            color.r * 255, color.g * 255, color.b * 255));
                }
            }

            return spheres;
        }

        std::vector<Plano> get_planes() {
            std::vector<Plano> planes;
            for (const auto& obj : scene.objects) {
                ColorData color = obj.material.color;
                if (obj.objType == "plane") {
                    Vetor pointVec = obj.vetorPointData.at("point_on_plane");
                    Ponto3d point(pointVec.getX(), pointVec.getY(), pointVec.getZ());
                    Vetor normalVec = obj.vetorPointData.at("normal");
                    Vector3d normal(normalVec.getX(), normalVec.getY(), normalVec.getZ());
                    
                    planes.push_back(Plano(point, normal, 
                                        color.r * 255, color.g * 255, color.b * 255));
                }
            }
            return planes;
        }
};

void renderizarDinamico(const Camera& cam, const Cena& cena) {
    std::ofstream file("out.ppm");
    file << "P3\n" << WIDTH_RES << " " << HEIGHT_RES << "\n255\n";
    
    // Render loop
    for (int i = 0; i < HEIGHT_RES; i++) {
        for (int j = 0; j < WIDTH_RES; j++) {
            Ray ray = cam.generate_ray(i, j);
            
            double closest_t = -1.0;
            int r = 0, g = 0, b = 0;
            
            // Check spheres
            for (const auto& sphere : cena.esferas) {
                double t = sphere.intersect(ray);
                if (t > 0 && (closest_t < 0 || t < closest_t)) {
                    closest_t = t;
                    r = sphere.R;
                    g = sphere.G;
                    b = sphere.B;
                }
            }
            
            // Check planes
            for (const auto& plane : cena.planos) {
                double t = plane.intersect(ray);
                if (t > 0 && (closest_t < 0 || t < closest_t)) {
                    closest_t = t;
                    r = plane.R;
                    g = plane.G;
                    b = plane.B;
                }
            }
            
            if (closest_t > 0) {
                file << r << " " << g << " " << b << " ";
            } else {
                file << "0 0 0 ";
            }
        }
        file << "\n";
    }
    file.close();
}

int main() {
    Ponto3d lookfrom = Ponto3d(scene.camera.lookfrom.getX(),
                                scene.camera.lookfrom.getY(),
                                scene.camera.lookfrom.getZ());
                                
    Ponto3d lookat = Ponto3d(scene.camera.lookat.getX(),
                            scene.camera.lookat.getY(),
                            scene.camera.lookat.getZ());
    
    Vector3d up = Vector3d(scene.camera.upVector.getX(),
                            scene.camera.upVector.getY(),
                            scene.camera.upVector.getZ());                             

    Camera cam(lookfrom, lookat, up, scene.camera.screen_distance);

    renderizarDinamico(cam, scene);
    return 0;
}
