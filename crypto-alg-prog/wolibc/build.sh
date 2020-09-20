#!/bin/sh
g++ -nostdlib -fno-stack-protector start.S -o wolibc mywrite.S main.cpp
