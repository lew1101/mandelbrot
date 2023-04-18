cmake_minimum_required(VERSION 3.0.0)
project(
    "Mandelbrot"
    VERSION 0.2.0
    LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wpedantic -Wextra -fexceptions)

add_executable(mandelbrot mandelbrot.cpp)
target_include_directories(mandelbrot PUBLIC ${PROJECT_SOURCE_DIR}/include)
