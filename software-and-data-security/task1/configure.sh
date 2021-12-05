#!/bin/bash

cur_dir=`dirname "$0"`

[[ -d $cur_dir/build ]] && rm -r $cur_dir/build
mkdir build

cmake \
    -S $cur_dir \
    -B $cur_dir/build \
    -D CMAKE_BUILD_TYPE=Release \
    -D CMAKE_CXX_FLAGS="-static"
