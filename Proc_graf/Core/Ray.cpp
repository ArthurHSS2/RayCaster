#include "Ray.hpp"

Point3d Ray::at(double t) const 
{
    return origin + (direction * t);
}

Point3d Ray::get_origin() const
{
    return origin;
}

Vector3d Ray::get_dir() const
{
    return direction;
}