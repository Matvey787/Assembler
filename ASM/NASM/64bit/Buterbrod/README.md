# Mandelbrot

## Description

The Mandelbrot project was created to show how much you can influence code optimization with the  
help of different optimizations, in particular, vectorization is realized here.  
The project consists of three parts:  

1. A naive [version](buterbrod_spd0.cpp) of Mandelbrot set drawing.
2. The [first version](buterbrod_spd1.cpp) of the vectorization implementation.
3. The [second version](buterbrod_spd2.0.cpp) or another [second version](buterbrod_spd2myIntr.cpp)  
with my inline intrinsics of the vectorization implementation, sweep factor 4×1.
4. The [third version](buterbrod_spd2.1.cpp) of the vectorization implementation, sweep factor 7×1.
5. The [final version](buterbrod_spd2trueIntr.cpp) of the vectorization implementation with real  
intrinsics library `<immintrin.h>`.

## Speed

Distribution: `Ubuntu 14.2.0`  
Linux version: `Linux version 6.11.0`  
CPU: `Intel Core i5 9300H (2.40 GHz)`  
> ![NOTE]
> The speed of the program is measured in fps (frames per second).

| Compiler Flags       | Naive Version | First Version | Vectorized (`no real AVX`, `4×1`) | Vectorized (`no real AVX`, `7×1`) | Vectorized (`real AVX`) |
|----------------------|---------------|---------------|----------------------------------|----------------------------------|------------------------|
| `g++ -O2`           | 22-23         | 39            | 52-53                            | 17                               | 59-60                  |
| `g++ -O3`           | 23            | 50-51         | 47-48                            | 42                               | 60-61                  |
| `clang++ -O2`       | 21-22         | 53            | 59-60                            | 78-79                            | 60-61                  |
| `clang++ -O3`       | 22            | 53            | 58-60                            | 81                               | 60                     |
