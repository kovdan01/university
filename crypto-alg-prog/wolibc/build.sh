#!/bin/sh
clang++ -nostdlib -fno-stack-check -fno-stack-protector start.S -o wolibc mywrite.S main.cpp
