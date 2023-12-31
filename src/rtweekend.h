#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <random>
#include <limits>
#include <memory>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // returns a random real between [0, 1).
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline double random_double(double min, double max) {
    // between [min, max)
    return min + (max - min) * random_double();
}

// Common headers the we will use everywhere

#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif
