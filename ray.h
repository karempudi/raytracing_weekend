#ifndef RAY_H
#define RAY_H

#include "vec3.h"


// Ray Class
//
// P(t) = A + tb, where A is the origin point
// of the ray and b is the direction of the ray
// t is sort of the scale factor for the direction
// A, b are vectors
//
class ray {

    private:
        point3 orig;
        vec3 dir;
    public:
        ray() {}

        ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

        // return origin A
        point3 origin() const { 
            return orig; 
        }
        // return direction b
        vec3 direction() const {
            return dir;
        }

        point3 at(double t) const {
            return orig + t * dir;
        }
};

#endif

