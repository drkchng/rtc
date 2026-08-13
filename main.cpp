#include <iostream>

int main() {
   int img_width = 256;
   int img_height = 256;

   std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";
   for (int i = 0; i < img_height; i++) {
      std::clog << "\rScanlines remaining: " << (img_height - i) << ' ' <<std::flush;
      for (int j = 0; j < img_width; j++) {
         auto r = (i+j)/2;
         auto g = (i+j)/2;
         auto b = (i+j)/2;

         std::cout << r << ' ' << g << ' ' << b << '\n';
      }
   }
   std::clog << "\rDone.\n";
}
