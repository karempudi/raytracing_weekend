#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"

#include <iostream>

class camera {

    public:
        double aspect_ratio = 1.0; // ration of image width to height
        int image_width = 100; // in pixels

        void render(const hittable& world) {
            initialize();// why do we need to intialize everytime we render

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for(int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for(int i = 0; i < image_width; i++) {
                    auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                    auto ray_direction = pixel_center - center;
                    ray r(center, ray_direction);

                    color pixel_color = ray_color(r, world);  
                    write_color(std::cout, pixel_color);
                }
            }
            std::clog << "\rDone.                   \n";
         }

   private:
        int image_height; // Rendered image height in pixels 
        point3 center; // camera center
        point3 pixel00_loc; // location of pixel 0, 0 center
        vec3 pixel_delta_u; // vector to move from one pixel to next in x
        vec3 pixel_delta_v; // vector to move from one pixel to next in y

        void initialize() {
            image_height = static_cast<int>(image_width/ aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            center = point3(0, 0, 0);

            // Determine viewport dimensions.
            //
            auto focal_length = 1.0;
            auto viewport_height = 2.0;
            auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

            // Calculate the vector across the horizontal and down the
            // vertical viewport edges
            auto viewport_u = vec3(viewport_width, 0, 0);
            auto viewport_v = vec3(0, -viewport_height, 0);

            // delta vectors to move from one pixel to the other
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            auto viewport_upper_left = center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
            

        }

        color ray_color(const ray& r, const hittable& world) const {

             hit_record rec;

             // if the ray hits, get the normal and use 
             // it to generate color
             if(world.hit(r, interval(0, infinity), rec)) {
                 return 0.5 * (rec.normal + color(1, 1, 1));
             }

             // if not the set some color
             vec3 unit_direction = unit_vector(r.direction());
             auto a = 0.5 * (unit_direction.y() + 1.0);

             return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
                   

        }

};

#endif

