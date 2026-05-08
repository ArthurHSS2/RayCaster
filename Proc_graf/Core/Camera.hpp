#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.hpp"

class Camera {
    Point3d C, M;
    Vector3d U, V, W;
    Vector3d Up;
    double dist;
    Vector3d direcao;
    double aspect_ratio;

	public:
        int height, width;

        Camera(Point3d pos, Point3d mira, Vector3d Up, double dist, int height, int width);

        Ray generate_ray(int row, int col) const;


};

#endif