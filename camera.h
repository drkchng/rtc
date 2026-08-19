#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
   public:
      double aspect_ratio = 1.0;
      int img_width = 100;
      int samples_per_pixel = 10;
      int max_depth = 10;

      double vfov = 90;
      point3 lookfrom = point3(0.0, 0.0, 0.0);
      point3 lookat = point3(0.0, 0.0, -1.0);
      vec3 vup = vec3(0.0, 1.0, 0.0);

      void render(const hittable& world) {
         initialize();
         
         std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
         for (int j = 0; j < img_height; j++) {
            std::clog << "\rScanlines remaining: " << (img_height - j) << ' ' <<std::flush;
            for (int i = 0; i < img_width; i++) {
               color pixel_color(0, 0, 0);
               for (int sample = 0; sample < samples_per_pixel; sample++) {
                  ray r = get_ray(i, j);
                  pixel_color += ray_color(r, max_depth, world);
               }

               write_color(std::cout, pixel_samples_scale * pixel_color);
            }
         }

         std::clog << "\rDone.\n";
      }
   private:
      int img_height;
      double pixel_samples_scale;
      point3 center;
      point3 pixel00_loc;
      vec3 pixel_delta_u;
      vec3 pixel_delta_v;
      vec3 u, v, w;

      void initialize() {
         img_height = int(img_width/ aspect_ratio);
         img_height = (img_height < 1) ? 1: img_height;

         pixel_samples_scale = 1.0 / samples_per_pixel;

         center = lookfrom;

                             // viewport dimensions
         auto focal_length = (lookfrom - lookat).length();
         auto theta = degrees_to_radians(vfov);
         auto h = std::tan(theta/2);
         auto vp_height = 2 * h * focal_length;
         auto vp_width = vp_height * (double(img_width)/img_height);

                             // Calculate the u, v, w unit basis vectors for the camera frame
         w = unit_vec3(lookfrom - lookat);
         u = unit_vec3(cross(vup, w));
         v = cross(w, u);
                             // Calculate edge to edge vectors
         auto vp_u = vp_width * u;
         auto vp_v = vp_height * -v;

                             // Calculate pixel delta vectors
         pixel_delta_u = vp_u / img_width;
         pixel_delta_v = vp_v / img_height;

                             // Upper left pixel.
         auto vp_upper_left = center - (focal_length * w) - vp_u/2 -vp_v/2;
         pixel00_loc = vp_upper_left + (pixel_delta_u + pixel_delta_v)/2;
      }
   
      ray get_ray(int i, int j) const {
                             // Construct a camera ray from the origin and directed at a randomly
                             // sampled point around (i,j)
         auto offset = sample_square();
         auto pixel_sample = pixel00_loc + 
                             ((i + offset.x()) * pixel_delta_u) + 
                             ((j + offset.y()) * pixel_delta_v);

         auto ray_origin = center;
         auto ray_direction = pixel_sample - ray_origin;

         return ray(ray_origin, ray_direction);
      }

      vec3 sample_square() const {
                             // Returns the vector to a random point in the -.5, +.5 square.
         return vec3(random_double() - 0.5, random_double() - 0.5, 0.0);
      }

      color ray_color(const ray& r, int depth, const hittable& world) const {
         if (depth <= 0)
            return color(0.0, 0.0, 0.0);;

         hit_record rec;

         if (world.hit(r, interval(0.00001, infinity), rec)) {
            ray scattered;
            color attenuation;
            if (rec.mat->scatter(r, rec, attenuation, scattered))
               return attenuation * ray_color(scattered, depth-1, world);

            return color(0.0, 0.0, 0.0);;
         }

         vec3 unit_dir = unit_vec3(r.direction());
         auto a = 0.5 * (unit_dir.y() + 1.0);
         return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
      }

};

#endif