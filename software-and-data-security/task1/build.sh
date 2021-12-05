#!/bin/bash

cur_dir=`dirname "$0"`

cmake --build $cur_dir/build --target clean
cmake --build $cur_dir/build --target all

binary_name="kovalev-task1"
strip -s  $cur_dir/build/$binary_name
upx -9 -v $cur_dir/build/$binary_name
