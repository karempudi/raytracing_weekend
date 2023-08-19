#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

// color is a just a vector3 of values between (0, 1)
// that are casted into ints
using color = vec3;

void write_color(std::ostream &out, color pixel_color) {
    // write out color to out
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}


#endif
