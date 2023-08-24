#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"
#include "material.h"

#include <iostream>

class camera {

    public:
        double aspect_ratio = 1.0; // ration of image width to height
        int image_width = 100; // in pixels
        int samples_per_pixel = 10; // count of random samples per pixel
        int max_depth = 10; // Maximum number of ray bounces into scene

        double vfov = 90; // 90 degress view angle (field of view)
        point3 lookfrom = point3(0, 0, -1); // Point camera is looking from
        point3 lookat = point3(0, 0, 0); // Point camera is looking at
        vec3 vup = vec3(0, 1, 0); // Camera-relative "up" direction

        double defocus_angle = 0; // Variation angle of rays through each pixel
        double focus_dist = 10; // Distance from camera lookfrom point to the plane of perfect focus

        void render(const hittable& world) {
            initialize();// why do we need to intialize everytime we render

            std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

            for(int j = 0; j < image_height; j++) {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for(int i = 0; i < image_width; i++) {

                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        // randomly get a ray surrounding the (i, j) pixel
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    } 
                    write_color(std::cout, pixel_color, samples_per_pixel);
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
        vec3 u, v, w; // Camera frame basis vectors
        vec3 defocus_disk_u; // Defocus disk horizontal radius
        vec3 defocus_disk_v; // Defocus disk vertical radius

        void initialize() {
            image_height = static_cast<int>(image_width/ aspect_ratio);
            image_height = (image_height < 1) ? 1 : image_height;

            center = lookfrom;

            // Determine viewport dimensions.
            //auto focal_length = (lookfrom - lookat).length();
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

            // Calculate u, v, w unit basis vectors for the camera coordinate frame.
            w = unit_vector(lookfrom - lookat); 
            u = unit_vector(cross(vup, w));
            v = cross(w, u);

            // Calculate the vector across the horizontal and down the
            // vertical viewport edges
            auto viewport_u = viewport_width * u; // Vector across viewport horizontal edge
            auto viewport_v = viewport_height * -v; // Vector down viewport vertical edge

            // delta vectors to move from one pixel to the other
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel.
            auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
            
            // Calculate the camera defocus disk basis vectors.
            auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;

        }

        ray get_ray(int i, int j) const {
            // Get a randomly sampled camera ray for pixel at location i, j.

            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto pixel_sample = pixel_center + pixel_sample_square();

            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);

        }

        point3 defocus_disk_sample() const {
            // Returns a random point in the camera defocus disk.
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        vec3 pixel_sample_square() const {
            // Returns a random point in square surrounding a pixel at the origin.
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();
            return (px * pixel_delta_u) + (py * pixel_delta_v);
            
        }

        color ray_color(const ray& r, int depth, const hittable& world) const {

             hit_record rec;

             // If we exceed the depth, we return black
             if (depth <= 0)
                 return color(0, 0, 0);

             // if the ray hits, get the normal and use 
             // it to generate color
             if(world.hit(r, interval(0.001, infinity), rec)) {
                 ray scattered;
                 color attenuation;

                 if(rec.mat->scatter(r, rec, attenuation, scattered)) 
                     // call the color again on the scattered 
                     return attenuation * ray_color(scattered, depth-1, world);
                 return color(0, 0, 0);
             }

             // if not the set some color
             vec3 unit_direction = unit_vector(r.direction());
             auto a = 0.5 * (unit_direction.y() + 1.0);

             return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);

        }

};

#endif

