#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

color ray_color(const ray& r) {
    vec3 unit_direction = unit_vector(r.direction());
    // make a between -1.0 and 1.0 with normalizing them
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(0.0, 0.0, 1.0)  + a * color(0.2, 0.7, 0.5);
}


int main() {

    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Image height based on aspect ratio, or a minimum of 1
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    //std::cout << "Height: " << image_height << " Width: " << image_width << '\n';

    // Camera, just set the height to 2.0 and 
    // calculate width to a double value based on 
    // final image_height
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
    auto camera_center = point3(0, 0, 0);


    // Horizontal and vertical vectors on the viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // calculate delta vectors from pixel to pixel.
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;


    //std::cout << "Viewport Height: " << viewport_height << " Width: " << viewport_width << '\n';


    //std::cout << "Pixel delta height: " << pixel_delta_u << " width: " << pixel_delta_v << '\n';

    // Calculate the location of the upper left pixel.
    // doing vector math with starting at origin and 
    // going in direction of focal_length to get to the 
    // center of the viewport and then to the upper left 
    // corner.
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render 
    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for(int i = 0; i < image_width; i++) {
            //caluclate pixel centers' location vectors for each of center in the viewport grid
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            // camera center is (0, 0, 0). so this won't make a
            // difference, but if we move the camera center,
            // it will make a difference.
            auto ray_direction = pixel_center - camera_center;

            // origin and ray direction
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                   \n";

}

