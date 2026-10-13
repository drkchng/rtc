# rtc

A small ray tracer written in C++20. It renders a scene of spheres with diffuse, metal, and glass materials and writes the image as a PPM file.

This project is based on the tutorial **[Ray Tracing in One Weekend](https://raytracing.github.io/books/RayTracingInOneWeekend.html)** by Peter Shirley, Trevor David Black, and Steve Hollasch. The commit history follows the book's progression, from the first PPM output through materials, camera, and depth of field to the final random scene.

## Features

- Sphere primitives and a list of hittable objects
- Materials: diffuse (Lambertian), metal with adjustable fuzz, and glass (dielectric) with refraction
- Anti-aliasing through multiple random samples per pixel
- Positionable camera with a configurable field of view
- Depth of field (defocus blur)
- A randomly generated scene as the final render
- Plain-text PPM (P3) output

## Build and run

Requires a C++20 compiler and [CMake](https://cmake.org/) 3.15 or later.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

The program writes the image to standard output and progress to standard error, so redirect the output to a file:

```bash
./build/rtc > image.ppm
```

Build in Release mode: the default CMake build is unoptimized and renders much more slowly.

**On Windows**, redirect with `cmd` rather than PowerShell. Windows PowerShell's `>` writes UTF-16, which most image viewers cannot read as PPM:

```bat
cmd /c "build\Release\rtc.exe > image.ppm"
```

(With a single-configuration generator such as MinGW Makefiles, the executable is `build\rtc.exe`.)

View the result in any viewer that supports PPM (for example GIMP or IrfanView), or convert it with ImageMagick: `magick image.ppm image.png`.

## Render settings

The scene and render settings are set in `main.cpp`. The defaults are a 1920-pixel-wide, 16:9 image at 500 samples per pixel with a maximum ray depth of 50, which takes a long time on a single core. For a quick preview, lower `cam.img_width` and `cam.samples_per_pixel` before building.

## Status

Only spheres are supported. Rendering is currently single-threaded on the CPU, so multithreading is the next planned improvement.
