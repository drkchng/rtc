#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
   public:
      double aspect_ratio = 1.0;
      double img_width = 100;
      void render(const hittable& world) {
         initialize();
         
         std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
         for (int i = 0; i < img_height; i++) {
            std::clog << "\rScanlines remaining: " << (img_height - i) << ' ' <<std::flush;
            for (int j = 0; j < img_width; j++) {
               auto pixel_center = pixel00_loc + (j * pixel_delta_u) + (i * pixel_delta_v);
               auto ray_direction = pixel_center - center;
               ray r(center, ray_direction);

               auto pixel_color = ray_color(r, world);
               write_color(std::cout, pixel_color);
            }
         }

         std::clog << "\rDone.\n";
      }
   private:
      int img_height;
      point3 center;
      point3 pixel00_loc;
      vec3 pixel_delta_u;
      vec3 pixel_delta_v;

      void initialize() {
         img_height = int(img_width/ aspect_ratio);
         img_height = (img_height <1) ? 1: img_height;

         center = point3(0, 0, 0);

                             // viewport dimensions
         auto focal_length = 1.0;
         auto vp_height = 2.0;
         auto vp_width = vp_height * (double(img_width)/img_height);

                             // Calculate edge to edge vectors
         auto vp_u = vec3(vp_width, 0, 0);
         auto vp_v = vec3(0,-vp_height,0);

                             // Calculate pixel delta vectors
         auto pixel_delta_u = vp_u / img_width;
         auto pixel_delta_v = vp_v / img_height;

                             // Upper left pixel.
         auto vp_upper_left = center - vec3(0, 0, focal_length) - vp_u/2 -vp_v/2;
         auto pixel00_loc = vp_upper_left + (pixel_delta_u + pixel_delta_v)/2; 
      }
   
   color ray_color(const ray& r, const hittable& world) const {
      hit_record rec;
      if (world.hit(r, interval(0, infinity), rec)) {
         return 0.5 * (rec.normal + color(1,1,1));
      }

      vec3 unit_dir = unit_vec3 (r.direction());
      auto a = 0.5 * (unit_dir.y() + 1.0);
      return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
   }

};

#endif