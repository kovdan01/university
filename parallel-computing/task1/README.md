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

**Intel ICPC:**

```
+--------------------+--------------+-----------+
|      operation     | ticks / iter | ns / iter |
+--------------------+--------------+-----------+
| independent scalar |       15.184 |   6.87676 |
+--------------------+--------------+-----------+
|  dependent scalar  |      22.6445 |   10.2556 |
+--------------------+--------------+-----------+
| independent vector |      12.1215 |   5.48973 |
+--------------------+--------------+-----------+
|  dependent vector  |       17.323 |   7.84543 |
+--------------------+--------------+-----------+
```

**Intel LLVM-based compiler:**

```
+--------------------+--------------+-----------+
|      operation     | ticks / iter | ns / iter |
+--------------------+--------------+-----------+
| independent scalar |     0.575047 |  0.260435 |
+--------------------+--------------+-----------+
|  dependent scalar  |      13.6274 |   6.17175 |
+--------------------+--------------+-----------+
| independent vector |      12.0852 |   5.47335 |
+--------------------+--------------+-----------+
|  dependent vector  |      17.3051 |   7.83742 |
+--------------------+--------------+-----------+
```

**GNU GCC:** unavailable due to lack of `_mm256_div_epi64`

## Task 1.2 - vectorizing and paralleling numeric integration

### Given

Function is defined as a table of its values in points *from*, *from + dx*, *from + 2dx*, ..., *to*. Here *dx = (to - from) / n*, where *n* is number of segments to split *\[from; to\]* segment.

### Results

CPU used: Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz

**Intel ICPC:**

```
+--------------------------+----------------+---------------+
|         operation        |  ticks / iter  |   ns / iter   |
+--------------------------+----------------+---------------+
|     integrate dummy      |      351976.69 |     169397.34 |
+--------------------------+----------------+---------------+
|  integrate omp parallel  |      300586.52 |     148018.84 |
+--------------------------+----------------+---------------+
|    integrate omp simd    |      352571.79 |     169682.05 |
+--------------------------+----------------+---------------+
```

**Intel LLVM-based compiler:** unavailable due to problems with OpenMP

**GNU GCC:**

```
+--------------------------+----------------+---------------+
|         operation        |  ticks / iter  |   ns / iter   |
+--------------------------+----------------+---------------+
|     integrate dummy      |     1848418.17 |     847134.62 |
+--------------------------+----------------+---------------+
|  integrate omp parallel  |      399752.25 |     191393.89 |
+--------------------------+----------------+---------------+
|    integrate omp simd    |      503556.24 |     237897.83 |
+--------------------------+----------------+---------------+
```
