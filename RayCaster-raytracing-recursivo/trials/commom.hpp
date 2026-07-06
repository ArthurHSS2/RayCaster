#ifndef COMMOM_HPP
#define COMMOM_HPP

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <array>
#include <vector>


// C++ Std Usings

using std::make_shared;
using std::shared_ptr;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers

#include "color.hpp"
#include "Ray.hpp"
#include "math/Vector3d.hpp"
#include "interval.hpp"

#endif
