# Mandelbrot

**Author:** [Kenneth Lew](https://github.com/lew1101)

![mandelbrot_one](./samples/mandelbrot_one.bmp)
![mandelbrot_set](./samples/mandelbrot_set.bmp)
![mandelbrot_two](./samples/mandelbrot_two.bmp)

## Usage

Run one of the following commands:

### Default Image

```shell
mandelbrot 
```

### Custom Image

```shell
mandelbrot [x_min] [x_max] [y_min] [y_max]
```

## Clone Repo

Make sure you have `git` installed on your machine. Then run:

```shell
git clone https://github.com/lew1101/mandelbrot.git
```

## Build

Make sure you have a C++ compiler and `CMake>=3.0.0` on your machine, then run the following command in the project directory to build in place:

```shell
cmake . && cmake --build .
```
