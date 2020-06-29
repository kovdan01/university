cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(final_project_dlib LANGUAGES CXX)

add_executable(${PROJECT_NAME} main.cpp color_reducer.cpp color_reducer.hpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

find_package(dlib REQUIRED)
find_package(Boost COMPONENTS COMPONENTS program_options REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE dlib::dlib Boost::program_options)
