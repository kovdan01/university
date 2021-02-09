cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

project(final_project_dlib LANGUAGES CXX)

find_package(dlib REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

add_executable(${PROJECT_NAME}
    color_reducer.cpp
    color_reducer.hpp
    main.cpp
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_link_libraries(${PROJECT_NAME} PRIVATE Boost::program_options)
target_link_libraries(${PROJECT_NAME} PRIVATE dlib::dlib)

include(FeatureSummary)
feature_summary(WHAT ALL)
