#!/bin/sh
g++ -nostdlib -no-pie start.S -o wolibc mywrite.S main.cpp
