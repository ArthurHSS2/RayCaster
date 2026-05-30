// main.cpp — Ray tracer guiado por JSON
// Uso: ./image_generator <cena.json> > output.ppm

#include "math/Matrix4x4.hpp"
#include "color.hpp"
#include "commom.hpp"
#include "hittable.h"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "triangle.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "Scene/sceneParser.cpp"
#include "MeshReader/ObjReader.cpp"

#include <iostream>
#include <memory>
#include <string>
#include <vector>

// -----------------------------------------------------------------------
// Helpers
// -----------------------------------------------------------------------

static color toColor(const ColorData& c) {
    return color(c.r, c.g, c.b);
}

struct Mat { color ka, kd, ks; double ns; };

static Mat toMat(const MaterialData& m) {
    return { toColor(m.ka), toColor(m.color), toColor(m.ks), m.ns };
}

// Monta a matriz de transformação a partir da lista do JSON.
// O sceneParser coloca o primeiro triplet de cada transform em t.data,
// independente da chave ser "factors", "angle" ou "vector".
static Matrix4x4 buildTransform(const std::vector<TransformData>& transforms) {
    Matrix4x4 result;
    for (const auto& t : transforms) {
        Matrix4x4 step;
        if (t.tType == "scaling") {
            step = Matrix4x4::scaling(t.data.x, t.data.y, t.data.z);
        } else if (t.tType == "translation") {
            step = Matrix4x4::translation(t.data.x, t.data.y, t.data.z);
        } else if (t.tType == "rotation") {
            if (t.data.x != 0.0) step = step * Matrix4x4::rotationX(t.data.x);
            if (t.data.y != 0.0) step = step * Matrix4x4::rotationY(t.data.y);
            if (t.data.z != 0.0) step = step * Matrix4x4::rotationZ(t.data.z);
        }
        result = step * result;
    }
    return result;
}

// -----------------------------------------------------------------------
// Carrega um .obj e adiciona seus triângulos ao world
// -----------------------------------------------------------------------
static void addMesh(hittable_list& world,
                    const std::string& objPath,
                    const Matrix4x4& T,
                    bool hasTransform,
                    const Mat& mat)
{
    objReader reader(objPath);

    const auto& allVerts   = reader.getVertices();
    const auto& allNormals = reader.getNormals();
    // getFacePoints() retorna vector<vector<Point3d>> com os 3 vértices de cada face
    const auto& facePoints = reader.getFacePoints();

    int count = 0;
    for (const auto& face : facePoints) {
        if (face.size() < 3) continue;

        Point3d vA = face[0];
        Point3d vB = face[1];
        Point3d vC = face[2];

        // Normal da face (geométrica) como fallback
        Vector3d faceN = (vB - vA).produto_vetorial(vC - vA).normalizacao();
        Vector3d nA = faceN, nB = faceN, nC = faceN;

        // Se o .obj tiver normais na mesma quantidade que vértices, interpola por vértice
        // (ObjReader original não expõe índices de normal por face,
        //  então usamos a normal da face para todos os vértices)

        if (hasTransform) {
            vA = T.multiply_point(vA);
            vB = T.multiply_point(vB);
            vC = T.multiply_point(vC);
            nA = T.multiply_vector(nA).normalizacao();
            nB = T.multiply_vector(nB).normalizacao();
            nC = T.multiply_vector(nC).normalizacao();
        }

        world.add(std::make_shared<triangle>(
            vA, vB, vC,
            nA, nB, nC,
            mat.ka, mat.kd, mat.ks, mat.ns
        ));
        count++;
    }

    std::clog << "[mesh] " << objPath << ": " << count << " triangulos carregados.\n";
}

// -----------------------------------------------------------------------
// Constrói o world a partir do SceneData
// -----------------------------------------------------------------------
static void buildWorld(const SceneData& scene, hittable_list& world) {
    for (const auto& obj : scene.objects) {

        Mat mat = toMat(obj.material);
        bool hasTransform = !obj.transforms.empty();
        Matrix4x4 T = hasTransform ? buildTransform(obj.transforms) : Matrix4x4();

        // ---- ESFERA ----
        if (obj.objType == "sphere") {
            double radius = obj.numericData.count("radius")
                            ? obj.numericData.at("radius") : 1.0;
            Point3d center = obj.relativePos;
            if (hasTransform) {
                center = T.multiply_point(center);
                radius = T.multiply_vector(Vector3d(radius, 0, 0)).modulo();
            }
            world.add(std::make_shared<sphere>(
                center, radius, mat.ka, mat.kd, mat.ks, mat.ns));
        }

        // ---- PLANO ----
        else if (obj.objType == "plane") {
            Point3d  point  = obj.relativePos;
            Vector3d normal(0, 1, 0);
            if (obj.vetorPointData.count("normal"))
                normal = obj.vetorPointData.at("normal");
            if (hasTransform) {
                point  = T.multiply_point(point);
                normal = T.multiply_vector(normal).normalizacao();
            }
            world.add(std::make_shared<plane>(
                point, normal, mat.ka, mat.kd, mat.ks, mat.ns));
        }

        // ---- MESH ----
        else if (obj.objType == "mesh") {
            if (!obj.otherProperties.count("path")) {
                std::cerr << "[aviso] mesh sem campo \"path\", pulando.\n";
                continue;
            }
            try {
                addMesh(world, obj.otherProperties.at("path"), T, hasTransform, mat);
            } catch (const std::exception& e) {
                std::cerr << "[erro] " << e.what() << "\n";
            }
        }

        else {
            std::cerr << "[aviso] tipo desconhecido: \"" << obj.objType << "\", pulando.\n";
        }
    }
}

// -----------------------------------------------------------------------
// main
// -----------------------------------------------------------------------
int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <cena.json> > output.ppm\n";
        return 1;
    }

    SceneData scene;
    try {
        scene = SceneJsonLoader::loadFile(argv[1]);
    } catch (const std::exception& e) {
        std::cerr << "Erro ao carregar JSON: " << e.what() << "\n";
        return 1;
    }

    std::clog << "Cena: "
              << scene.objects.size()   << " objetos, "
              << scene.lightList.size() << " luzes, "
              << scene.camera.image_width << "x" << scene.camera.image_height << "\n";

    hittable_list world;
    buildWorld(scene, world);

    std::vector<PointLight> luzes;
    for (const auto& l : scene.lightList)
        luzes.emplace_back(l.pos, toColor(l.color));

    color luz_ambiente = toColor(scene.globalLight.color);

    camera cam;
    cam.image_width     = scene.camera.image_width;
    cam.aspect_ratio    = scene.camera.image_height > 0
                          ? double(scene.camera.image_width) / scene.camera.image_height
                          : 16.0 / 9.0;
    cam.lookfrom        = scene.camera.lookfrom;
    cam.lookat          = scene.camera.lookat;
    cam.vup             = scene.camera.upVector;
    cam.screen_distance = scene.camera.screen_distance;

    cam.render(world, luzes, luz_ambiente);

    return 0;
}