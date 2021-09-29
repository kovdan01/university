cmake_minimum_required(VERSION 3.5)

project(kovalev-lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(shared shared.hpp shared.cpp)

add_executable(encrypt encrypt.cpp)
target_link_libraries(encrypt shared)

add_executable(decrypt decrypt.cpp)
target_link_libraries(decrypt shared)

add_executable(convert_table convert_table.cpp)
