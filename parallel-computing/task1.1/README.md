# Task 1.1 - measuring CPU performance

## Given

Scalar operation is division of two 64-bit signed integers
Vector operation is `_mm256_div_epi64` from AVX2 extension (not available as intrinsic with regular gcc or clang, defined in libsvml, which is installed among with Intel ICC compiler). Performs four 64-bit signed integer divisions as one operation.

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

## Results

CPU used: Intel(R) Core(TM) i7-8750H CPU @ 2.20GHz

```
+--------------------+--------------+-----------+
|      operation     | ticks / iter | ns / iter |
+--------------------+--------------+-----------+
| independent scalar |      15.6974 |   7.10927 |
+--------------------+--------------+-----------+
|  dependent scalar  |         22.6 |   10.2354 |
+--------------------+--------------+-----------+
| independent vector |      12.0865 |   5.47393 |
+--------------------+--------------+-----------+
|  dependent vector  |      17.3771 |   7.87003 |
+--------------------+--------------+-----------+
```
