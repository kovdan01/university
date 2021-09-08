#!/bin/bash

cur_dir=`dirname "$0"`

[[ -d $cur_dir/build ]] && rm -r $cur_dir/build
[[ -d $cur_dir/out ]] && rm -r $cur_dir/out
mkdir -p $cur_dir/build/{debug,debug-asan-ubsan,release}
mkdir $cur_dir/out

cmake \
    -S $cur_dir \
    -B $cur_dir/build/debug-asan-ubsan \
    -D CMAKE_BUILD_TYPE=Debug \
    -D CMAKE_CXX_FLAGS="-g -fsanitize=address,undefined -fno-sanitize-recover=all"

cmake \
    -S $cur_dir \
    -B $cur_dir/build/debug \
    -D CMAKE_BUILD_TYPE=Debug

cmake \
    -S $cur_dir \
    -B $cur_dir/build/release \
    -D CMAKE_BUILD_TYPE=Release
