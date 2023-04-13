#include <bitmap/bitmap_image.hpp>

#include <array>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

constexpr int WIDTH = 1800;
constexpr int HEIGHT = 1200;

constexpr int ITER_MAX = 1000;
constexpr int BOUNDS = 1 << 8;

int main(int argc, char *argv[]) {
  bitmap_image image{WIDTH, HEIGHT};
  image.clear();

  long double CR_MIN = -2.25;
  long double CR_MAX = 0.75;
  long double CI_MIN = -1.0;
  long double CI_MAX = 1.0;

  if (argc == 5) {
    try {
      CR_MIN = std::stod(argv[1]);
      CR_MAX = std::stod(argv[2]);
      CI_MIN = std::stod(argv[3]);
      CI_MAX = std::stod(argv[4]);
    } catch (const std::invalid_argument &e) {
      std::cerr << "arguments must be in decimal format" << std::endl;
      return 1;
    }

    if (CR_MIN >= CR_MAX) {
      std::cerr << "x_min must be less than x_max" << std::endl;
      return 1;
    } else if (CI_MIN >= CI_MAX) {
      std::cerr << "x_min must be less than x_max" << std::endl;
      return 1;
    }

  } else if (argc != 1) {
    std::cerr << "argv takes 4 arguments: x_min, x_max, y_min, y_max"
              << std::endl;
    return 1;
  }

  const long double CR_STEP = (CR_MAX - CR_MIN) / WIDTH;
  const long double CI_STEP = (CI_MAX - CI_MIN) / HEIGHT;
  const long double LOG_2 = log(2);

  long double cr = CR_MIN;
  for (int x = 0; x < WIDTH; ++x, cr += CR_STEP) {

    long double ci = CI_MIN;
    for (int y = 0; y < HEIGHT; ++y, ci += CI_STEP) {

      int i = 0;
      long double temp_zr;
      long double zr = 0, zi = 0;

      // https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set#Continuous_(smooth)_coloring
      while ((zr * zr + zi * zi) < BOUNDS && i++ < ITER_MAX) {
        temp_zr = zr * zr - zi * zi + cr;
        zi = 2 * zr * zi + ci;
        zr = temp_zr;
      }

      if (i < ITER_MAX) {
        // https://www.paridebroggi.com/blogpost/2015/05/06/fractal-continuous-coloring/
        const long double z = std::log(zr * zr + zi * zi) / 2;
        const long double continuous_index =
            i + 1 - std::log(z / LOG_2) / LOG_2;

        // https://krazydad.com/tutorials/makecolors.php
        // channel = sin(frequency * continuous_index + phase) * center + delta)
        const rgb_t color{
            (unsigned char)(std::sin(0.17 * continuous_index + 0) * 115 + 140),
            (unsigned char)(std::sin(0.15 * continuous_index + 2) * 115 + 140),
            (unsigned char)(std::sin(0.2 * continuous_index + 4) * 115 + 140)};

        image.set_pixel(x, y, color);
      }
    }
  }

  image.save_image("mandelbrot.bmp");
}
