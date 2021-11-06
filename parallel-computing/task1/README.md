# Task 1

## How to build

```bash
cmake \
    -S . \
    -B build \
    -D CMAKE_CXX_COMPILER=icpc \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_CXX_FLAGS="-march=native"
cmake --build build --target all
```

You could change `icpc` to another compiler you want to use. Note that:

* GNU GCC cannot be used to compile task 1.1. Add `-D BUILD_TASK_1_1=OFF` option.
* Intel LLVM-based compiler cannot be used to compile task 1.2. Add `-D BUILD_TASK_1_2=OFF` option.

## Task 1.1 - measuring CPU performance

### Given

Scalar operation is division of two 64-bit signed integers

Vector operation is `_mm256_div_epi64` from AVX2 extension (not available as intrinsic with regular gcc or clang, defined in libsvml, which is installed among with Intel ICC compiler). Performs four 64-bit signed integer divisions as one operation.

### Results

CPU used: Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz

Measured with frequency locked to 2.20GHz and turbo-boost disabled

**Intel ICPC:**

```
+--------------------+--------------+-----------+
|      operation     | ticks / iter | ns / iter |
+--------------------+--------------+-----------+
| independent scalar |      28.4806 |   12.8986 |
+--------------------+--------------+-----------+
|  dependent scalar  |      42.3657 |   19.1871 |
+--------------------+--------------+-----------+
| independent vector |      22.7078 |   10.2842 |
+--------------------+--------------+-----------+
|  dependent vector  |      32.4164 |   14.6812 |
+--------------------+--------------+-----------+
```

**Intel LLVM-based compiler:**

```
+--------------------+--------------+-----------+
|      operation     | ticks / iter | ns / iter |
+--------------------+--------------+-----------+
| independent scalar |      1.08797 |   0.49274 |
+--------------------+--------------+-----------+
|  dependent scalar  |      25.4767 |   11.5383 |
+--------------------+--------------+-----------+
| independent vector |      24.1993 |   10.9597 |
+--------------------+--------------+-----------+
|  dependent vector  |       32.294 |   14.6258 |
+--------------------+--------------+-----------+
```

**GNU GCC:** unavailable due to lack of `_mm256_div_epi64`

## Task 1.2 - vectorizing and paralleling numeric integration

### Given

Function is defined as a table of its values in points *from*, *from + dx*, *from + 2dx*, ..., *to*. Here *dx = (to - from) / n*, where *n* is number of segments to split *\[from; to\]* segment.

### Results

CPU used: Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz

Measured with frequency locked to 2.20GHz and turbo-boost disabled

**Intel ICPC:**

```
+----------------------------+----------------+---------------+
|          operation         |  ticks / iter  |   ns / iter   |
+----------------------------+----------------+---------------+
|      integrate dummy       |    10845158.04 |    4932022.54 |
+----------------------------+----------------+---------------+
|     integrate omp simd     |    10828490.51 |    4924846.01 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 1  |    18179023.56 |    8253084.30 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 2  |     9589733.79 |    4361244.32 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 3  |     6985018.73 |    3181948.56 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 4  |     6216703.93 |    2834112.84 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 5  |     5939075.67 |    2708988.63 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 6  |     6061977.57 |    2766919.99 |
+----------------------------+----------------+---------------+
```

**Intel LLVM-based compiler:** unavailable due to problems with OpenMP

**GNU GCC:**

```
+----------------------------+----------------+---------------+
|          operation         |  ticks / iter  |   ns / iter   |
+----------------------------+----------------+---------------+
|      integrate dummy       |    34339403.18 |   15572950.02 |
+----------------------------+----------------+---------------+
|     integrate omp simd     |    10838969.24 |    4929447.89 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 1  |    34272412.32 |   15540118.80 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 2  |    17236436.64 |    7825217.52 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 3  |    11596463.97 |    5271108.65 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 4  |     8933118.66 |    4065504.48 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 5  |     7384450.33 |    3364648.08 |
+----------------------------+----------------+---------------+
|  integrate omp parallel 6  |     6384912.18 |    2913110.82 |
+----------------------------+----------------+---------------+
```
