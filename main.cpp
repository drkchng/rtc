#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

bool hit_sphere(const point3& center, double radius, const ray& r) {
   vec3 oc = center - r.origin();
   auto a = dot(r.direction(), r.direction());
   auto b = -2.0 * dot(r.direction(), oc);
   auto c = dot(oc, oc) - radius * radius;
   auto discriminant = b*b - 4*a*c;
   return (discriminant >= 0);
}
color ray_color(const ray& r) {
   if (hit_sphere(point3(0, 0, -1), 0.5, r))
      return color(1, 0 , 0);
      
   vec3 unit_dir = unit_vec3 (r.direction());
   auto a = 0.5 * (unit_dir.y() + 1.0);
   return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {
                             // image
   auto aspect_ratio = 16.0/9.0;
   int img_width = 400;
                             // Calculate img height. it should at least be 1.
   int img_height = int(img_width/ aspect_ratio);
   img_height = (img_height <1) ? 1: img_height;

                             // camera
   auto focal_length = 1.0;
   auto vp_height = 2.0;
   auto vp_width = vp_height * (double(img_width)/img_height);
   auto camera_center = point3(0,0,0);

                             // Calculate edge to edge vectors
   auto vp_u = vec3(vp_width, 0, 0);
   auto vp_v = vec3(0,-vp_height,0);

                             // Calculate pixel delta vectors
   auto pixel_delta_u = vp_u / img_width;
   auto pixel_delta_v = vp_v / img_height;

                             // Upper left pixel.
   auto vp_upper_left = camera_center - vec3(0, 0, focal_length) - vp_u/2 -vp_v/2;
   auto pixel00_loc = vp_upper_left + (pixel_delta_u + pixel_delta_v)/2;

                             // Render
   std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
   for (int i = 0; i < img_height; i++) {
      std::clog << "\rScanlines remaining: " << (img_height - i) << ' ' <<std::flush;
      for (int j = 0; j < img_width; j++) {
         auto pixel_center = pixel00_loc + (j * pixel_delta_u) + (i * pixel_delta_v);
         auto ray_direction = pixel_center - camera_center;
         ray r(camera_center, ray_direction);
         auto pixel_color = ray_color(r);
         /*auto pixel_color = color(double(i+j)/(img_height+img_width-2),
                                  double(i+j)/(img_height+img_width-2),
                                  double(i+j)/(img_height+img_width-2));*/
         write_color(std::cout, pixel_color);
      }
   }
   std::clog << "\rDone.\n";
}
