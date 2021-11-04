cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(task1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_library(tools tools.cpp tools.hpp)

add_executable(task1.1 task1.1.cpp)
target_link_libraries(task1.1 tools)

add_executable(task1.2 task1.2.cpp)
target_link_libraries(task1.2 tools)
target_link_libraries(task1.2 OpenMP::OpenMP_CXX)
