#include "main_utility.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {
                             // World
   hittable_list world;

   auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.55));
   world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

   for (int a = -12; a < 12; a++) {
      for (int b = -12; b < 12; b++) {
         auto choose_mat = random_double();
         point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

         if ((center - point3(4.0, 0.2, 0.0)).length() > 0.9) {
            shared_ptr<material> sphere_material;

            if (choose_mat < 0.8) {
               auto albedo = color::random() * color::random();
               sphere_material = make_shared<lambertian>(albedo);
               world.add(make_shared<sphere>(center, 0.2, sphere_material));
            } else if (choose_mat < 0.95) {
               auto albedo = color::random(0.5, 1.0);
               auto fuzz = random_double(0.0, 0.5);
               sphere_material = make_shared<metal>(albedo, fuzz);
               world.add(make_shared<sphere>(center, 0.2, sphere_material));
            } else {
               sphere_material = make_shared<dielectric>(1.5);
               world.add(make_shared<sphere>(center, 0.2, sphere_material));
            }
         }
      }
   }

   auto mat_back = make_shared<lambertian>(color(0.3, 0.05, 0.6));
   world.add(make_shared<sphere>(point3(-4.0, 1.0, 0.0), 1.0, mat_back));

   auto mat_mid = make_shared<dielectric>(1.5);
   world.add(make_shared<sphere>(point3(0.0, 1.0, 0.0), 1.0, mat_mid));

   auto mad_front = make_shared<metal>(color(0.6, 0.5, 0.5), 0.0);
   world.add(make_shared<sphere>(point3(4.0, 1.0, 0.0), 1.0, mad_front));

   camera cam;
   cam.aspect_ratio      = 16.0 / 9.0;
   cam.img_width       = 1920;
   cam.samples_per_pixel = 500;
   cam.max_depth         = 50;

   cam.vfov     = 20;
   cam.lookfrom = point3(13.0, 2.0, 3.0);
   cam.lookat   = point3(0.0, 0.0, 0.0);
   cam.vup      = vec3(0.0, 1.0, 0.0);

   cam.defocus_angle = 0.6;
   cam.focus_dist    = 10.0;

   cam.render(world);
}
