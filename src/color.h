#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

// color is a just a vector3 of values between (0, 1)
// that are casted into ints
using color = vec3;

void write_color(std::ostream &out, color pixel_color, int samples_per_pixel) {

    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Divide the color by the number of samples.

    auto scale = 1.0 / samples_per_pixel;

    r *= scale;
    g *= scale;
    b *= scale;

    // Write the translated [0, 255] value of each color component.
    static const interval intensity(0.000, 0.999);

    // write out color to out
    out << static_cast<int>(256 * intensity.clamp(r)) << ' '
        << static_cast<int>(256 * intensity.clamp(g)) << ' '
        << static_cast<int>(256 * intensity.clamp(b)) << '\n';
}


#endif