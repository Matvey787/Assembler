# Mandelbrot

## Description

The Mandelbrot project was created to show how much you can influence code optimization with the  
help of different optimizations, in particular, vectorization is realized here.  
The project consists of three parts:  

1. A naive [version](src/buterbrod_spd0.cpp) of Mandelbrot set drawing.
2. The [first version](src/buterbrod_spd1.cpp) of the vectorization implementation.
3. The [second version](src/buterbrod_spd2.0.cpp) or another [second version](src/buterbrod_spd2myIntr.cpp)  
with my inline intrinsics of the vectorization implementation, sweep factor 4×1.
4. The [third version](src/buterbrod_spd2.1.cpp) of the vectorization implementation, sweep factor 7×1.
5. The [final version](src/buterbrod_spd3trueIntr.cpp) of the vectorization implementation with real  
intrinsics library `<immintrin.h>`.

## Speed

Distribution: `Ubuntu 14.2.0`  
Linux version: `Linux version 6.11.0`  
CPU: `Intel Core i5 9300H (2.40 GHz)`  
> [!NOTE]
> The speed of the program is measured in fps (frames per second).

| Compiler Flags       | Naive Version | First Version | Vectorized (`no real AVX`, `4×1`) | Vectorized (`no real AVX`, `7×1`) | Vectorized (`real AVX`) |
|----------------------|---------------|---------------|----------------------------------|----------------------------------|------------------------|
| `g++ -O2`           | 22-23         | 39            | 52-53                            | 17                               | 59-60                  |
| `g++ -O3`           | 23            | 50-51         | 47-48                            | 42                               | 60-61                  |
| `clang++ -O2`       | 21-22         | 53            | 59-60                            | 78-79                            | 60-61                  |
| `clang++ -O3`       | 22            | 53            | 58-60                            | 81                               | 60                     |

![Diagram](imgs/fpsDiagram.svg)

## AVX functions used in the project

> [!Note]
> You need the `-mavx` flag to make it work.

AVX (Advanced Vector Extensions) is a set of instructions for x86/x86-64 processors that enables parallel computations on multiple data points simultaneously (SIMD — Single Instruction, Multiple Data). In this program, AVX is utilized to accelerate Mandelbrot set calculations by processing 4 `double` values in a single operation.

The table below lists the main AVX functions employed in the code:

| AVX Function Name          | AVX Function Parameters                      | Return Value            | Brief Description of What It Does                     |
|----------------------------|---------------------------------------------|-------------------------|-------------------------------------------------------|
| `_mm256_loadu_pd`          | `const double* ptr`                         | `__m256d`               | Loads 4 `double` values from memory into an AVX register |
| `_mm256_set1_pd`           | `double a`                                  | `__m256d`               | Creates an AVX register with 4 copies of a single value |
| `_mm256_mul_pd`            | `__m256d a, __m256d b`                      | `__m256d`               | Multiplies 4 pairs of `double` values element-wise    |
| `_mm256_add_pd`            | `__m256d a, __m256d b`                      | `__m256d`               | Adds 4 pairs of `double` values element-wise          |
| `_mm256_sub_pd`            | `__m256d a, __m256d b`                      | `__m256d`               | Subtracts 4 pairs of `double` values element-wise     |
| `_mm256_cmp_pd`            | `__m256d a, __m256d b, int imm8`            | `__m256d`               | Compares 4 pairs of `double` values (here, <=)        |
| `_mm256_movemask_pd`       | `__m256d a`                                 | `int`                   | Converts comparison results into a bit mask           |
| `_mm_loadu_si128`          | `__m128i const* mem_addr`                   | `__m128i`               | Loads 4 `int` values into an SSE register             |
| `_mm_add_epi32`            | `__m128i a, __m128i b`                      | `__m128i`               | Adds 4 pairs of `int` values element-wise             |
| `_mm_storeu_si128`         | `__m128i* mem_addr, __m128i a`              | `void`                  | Stores 4 `int` values from a register into memory     |

These functions enable simultaneous computation on 4 points of the Mandelbrot set, significantly improving performance compared to sequential operations.

These functions enable simultaneous computation on 4 points of the Mandelbrot set, significantly improving performance compared to sequential operations.