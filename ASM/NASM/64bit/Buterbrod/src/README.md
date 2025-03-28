# Mandelbrot

## Description

The Mandelbrot project was created to show how much you can influence code optimization with the  
help of different optimizations, in particular, vectorization is realized here.  
The project consists of three parts:  

1. A naive [version](buterbrod_spd0.cpp) of Mandelbrot set drawing.
2. The [first version](buterbrod_spd1.cpp) of the vectorization implementation.
3. The [second version](buterbrod_spd2.cpp) or another [second version](buterbrod_spd2myIntr.cpp)  
with my inline intrinsics of the vectorization implementation.
4. The [final version](buterbrod_spd2trueIntr.cpp) of the vectorization implementation with real  
intrinsics library `<immintrin.h>`.

## Speed

Distribution: `Ubuntu 14.2.0`
Linux version: `Linux version 6.11.0`
CPU: `Intel Core i5 9300H (2.40 GHz)`
> NOTE
> The speed of the program is measured in fps (frames per second).

|                      | Naive version | First version | Vectorized version (`no real AVX`) | Vectorized version (`real AVX`) |
|----------------------|---------------|---------------|------------------------------------|---------------------------------|
| `g++ -O2`            | 22-23         | -             | 52-53                              | 59-60                           |
|----------------------|---------------|---------------|------------------------------------|---------------------------------|
| `g++ -O3`            | 23            | -             | 47-48                              | 60-61                           |
|----------------------|---------------|---------------|------------------------------------|---------------------------------|
| `clang++ -O2`        | 21-22         | -             | 59-60                              | 60-61                           |
|----------------------|---------------|---------------|------------------------------------|---------------------------------|
| `clang++ -O3`        | 22            | -             | 58-60                              | 60                              |
