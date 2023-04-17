#include <cmath>
#include <iostream>
#include <stdexcept>

#include <bitmap/bitmap_image.hpp>

constexpr int WIDTH = 1800;
constexpr int HEIGHT = 1200;

constexpr int ITER_MAX = 1000;
constexpr int BOUNDS = 1 << 8;

int main(int argc, char *argv[]) {
  bitmap_image image{WIDTH, HEIGHT};
  image.clear();

  // default arguments
  long double C_REAL_MIN = -2.25;
  long double C_REAL_MAX = 0.75;
  long double C_IMAG_MIN = -1.0;
  long double C_IMAG_MAX = 1.0;

  if (argc == 5) {
    // parse arguments
    try {
      C_REAL_MIN = std::stod(argv[1]);
      C_REAL_MAX = std::stod(argv[2]);
      C_IMAG_MIN = std::stod(argv[3]);
      C_IMAG_MAX = std::stod(argv[4]);
    } catch (const std::invalid_argument &e) {
      std::cerr << "arguments must be in decimal format" << std::endl;
      return 1;
    }

    // check if arguments are valid
    if (C_REAL_MIN >= C_REAL_MAX) {
      std::cerr << "x_min must be less than x_max" << std::endl;
      return 1;
    } else if (C_IMAG_MIN >= C_IMAG_MAX) {
      std::cerr << "y_min must be less than y_max" << std::endl;
      return 1;
    }

  } else if (argc != 1) {
    std::cerr << "argv takes 4 arguments: x_min, x_max, y_min, y_max"
              << std::endl;
    return 1;
  }

  // calculate step per pixel in vertical and horizontal direction
  const long double C_REAL_STEP = (C_REAL_MAX - C_REAL_MIN) / WIDTH;
  const long double C_IMAG_STEP = (C_IMAG_MAX - C_IMAG_MIN) / HEIGHT;

  // precalculate log(2) to make calculations faster
  const long double LOG_2 = log(2);

  long double c_real = C_REAL_MIN;
  for (int x = 0; x < WIDTH; ++x, c_real += C_REAL_STEP) {

    long double c_imag = C_IMAG_MIN;
    for (int y = 0; y < HEIGHT; ++y, c_imag += C_IMAG_STEP) {

      int iterations = 0;
      long double temp_z_real;
      long double z_real = 0, z_imag = 0;

      // https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set#Continuous_(smooth)_coloring
      while ((z_real * z_real + z_imag * z_imag) < BOUNDS &&
             iterations++ < ITER_MAX) {
        // Mandelbrot set: all complex numbers c where z_n = z_{n-1}^2 + c does
        // not diverge towards infinity

        // (a+bi)^2 = (a^2 - b^2) + 2abi
        temp_z_real = (z_real * z_real) - (z_imag * z_imag) + c_real;
        z_imag = (2 * z_real * z_imag) + c_imag;

        z_real = temp_z_real;
      }

      if (iterations < ITER_MAX) {
        // https://www.paridebroggi.com/blogpost/2015/05/06/fractal-continuous-coloring/
        const long double zeta =
            log(z_real * z_real + z_imag * z_imag) / 2; // magic
        const long double continuous_index =
            iterations + 1 - log(zeta / LOG_2) / LOG_2; // also magic

        // https://krazydad.com/tutorials/makecolors.php
        // channel = sin(frequency * continuous_index + phase) * center + delta)
        const rgb_t px_color{
            (unsigned char)(sin(0.17 * continuous_index + 0) * 115 + 140), // r
            (unsigned char)(sin(0.15 * continuous_index + 2) * 115 + 140), // g
            (unsigned char)(sin(0.2 * continuous_index + 4) * 115 + 140)   // b
        };

        image.set_pixel(x, y, px_color);
      }
    }
  }

  image.save_image("mandelbrot_output.bmp");
}
