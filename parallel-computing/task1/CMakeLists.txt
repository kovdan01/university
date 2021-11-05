cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(task1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(tools tools.cpp tools.hpp)

option(BUILD_TASK_1_1 "Build task 1.1 (requires Intel compiler)" ON)
option(BUILD_TASK_1_2 "Build task 1.2 (requires OpenMP)"         ON)

if(BUILD_TASK_1_1)
    if(NOT(CMAKE_CXX_COMPILER_ID STREQUAL "Intel" OR CMAKE_CXX_COMPILER_ID STREQUAL "IntelLLVM"))
        message(WARNING "Compiler is not ICPC or Intel LLVM, you might face problems when compiling")
    endif()
    add_executable(task1.1 task1.1.cpp)
    target_link_libraries(task1.1 tools)
endif()

if(BUILD_TASK_1_2)
    find_package(OpenMP REQUIRED)
    add_executable(task1.2 task1.2.cpp)
    target_link_libraries(task1.2 tools)
    target_link_libraries(task1.2 OpenMP::OpenMP_CXX)
endif()
