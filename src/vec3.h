#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {

    public:
        double e[3]; // the vector in the phyiscs sense

        // empty constructor
        vec3() : e{0, 0, 0} {}
        // initalize with values
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        // return xyz
        double x() const { return e[0];}
        double y() const { return e[1];}
        double z() const { return e[2];}

        // negative operator
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
        // indexing to value 
        double operator[](int i) const { return e[i];}
        // overload if you want reference
        double& operator[](int i) { return e[i];}

        // add vector 
        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0];
            e[1] += v.e[1];
            e[2] += v.e[2];
            return *this;
        }

        // scale vector
        vec3& operator*=(double t) {
            e[0] *= t;
            e[1] *= t;
            e[2] *= t;
            return *this;
        }
        // divide vector
        vec3& operator/=(double t) {
            return *this *= 1/t;
        }

        // norm2/length
        double length () const {
            return sqrt(length_squared());
        }
        // x^2 + y^2 + z^2
        double length_squared() const {
            return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
        }
        // random vector
        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), 
                        random_double(min, max),
                        random_double(min, max));
        }

        bool near_zero() const {
            // Return true if the vector is close to zero in all dimensions
            auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }

};

// alias name to use point3 instead of vec3
using point3 = vec3;

// utility functions
//
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// add two vectors
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// subtract two vectors
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}
// element-wise multiplication
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}
// scaleed vector
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t * v.e[0], t * v.e[1], t * v.e[2]);
}
// otherway round multiplication
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

// divided vector
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

// vector dot product
inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

// vector cross product
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}
// generate a random vector that fall in a sphere of radius 1.
inline vec3 random_in_unit_sphere() {
    while(true) {
        auto p = vec3::random(-1, 1);
        if (p.length_squared() < 1)
            return p;
    }
}

// make it a unit vector
inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if(dot(on_unit_sphere, normal) > 0.0) 
        return on_unit_sphere;
    else
        return -on_unit_sphere;

}

// get the reflected vector v, against the normal direction n
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v, n) * n;
}

// refract a vector v, over a normal n, with refractive index ratio
// at the boundary
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}


#endif

