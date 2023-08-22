#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

class hit_record {
    public:
        point3 p; // point on the object that is hit with the ray
        vec3 normal; // nomral vector on the surface at the hit point
        double t;

        bool front_face;

        void set_face_normal(const ray& r, const vec3& outward_normal) {
            
            front_face = dot(r.direction(), outward_normal) < 0;
            normal = front_face ? outward_normal: -outward_normal;
        }
};

class hittable {
    public:
        virtual ~hittable() = default;

        virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

};

#endif
