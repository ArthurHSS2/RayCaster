#include "Ray.hpp"

#ifndef CAMERA_H

#define CAMERA_H

class Camera {
    Point3d C, M;
    Vector3d U, V, W;
    Vector3d Up;
    double dist;
    Vector3d direcao;
    double aspect_ratio; 
    int width, height;

	public:
        Camera(Point3d pos, Point3d mira, Vector3d Up, double dist, int width, int height);

        Ray generate_ray(int row, int col) const;
};

#endif