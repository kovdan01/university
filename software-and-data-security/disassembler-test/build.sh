#!/bin/bash

cur_dir=`dirname "$0"`

cmake --build $cur_dir/build/debug-asan-ubsan --target all
cmake --build $cur_dir/build/debug --target all
cmake --build $cur_dir/build/release --target all

binary_name="disassembler-test"

process_binary()
{
    cp $cur_dir/build/$1/$binary_name $cur_dir/out/$1
    cp $cur_dir/out/$1 $cur_dir/out/$1-stripped
    strip -s $cur_dir/out/$1-stripped
}

process_binary "debug-asan-ubsan"
process_binary "debug"
process_binary "release"
