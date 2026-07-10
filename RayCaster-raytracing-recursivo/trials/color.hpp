#ifndef COLOR_H
#define COLOR_H

#include "commom.hpp"

using color = Vector3d;

void write_color(std::ostream& out, const color& pixel_color) {
    // 1. Tone Mapping (Reinhard) - maps HDR to LDR
    // Apply to each channel individually
    double r = pixel_color.x;
    double g = pixel_color.y;
    double b = pixel_color.z;
    
    // Optional: Exposure adjustment (e.g., multiply by 0.8 to brighten/darken)
    // double exposure = 1.0;
    // r *= exposure; g *= exposure; b *= exposure;
    
    // Reinhard tone mapping: x / (x + 1)
    r = r / (r + 1.0);
    g = g / (g + 1.0);
    b = b / (b + 1.0);
    
    // 2. Gamma Correction (assume display gamma 2.2)
    const double gamma = 2.2;
    r = pow(r, 1.0/gamma);
    g = pow(g, 1.0/gamma);
    b = pow(b, 1.0/gamma);
    
    // 3. Clamp just in case of floating point errors (nan/inf or negative)
    if (r < 0) r = 0; if (r > 1) r = 1;
    if (g < 0) g = 0; if (g > 1) g = 1;
    if (b < 0) b = 0; if (b > 1) b = 1;
    
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
#endif
