cmake_minimum_required(VERSION 3.5)

project(attack-emulation LANGUAGES CXX)

add_library(shared shared.hpp shared.cpp)
target_compile_features(shared PRIVATE cxx_std_20)

add_executable(defender defender.cpp)
target_link_libraries(defender PRIVATE shared)
target_compile_features(defender PRIVATE cxx_std_20)

add_executable(attacker attacker.cpp)
target_link_libraries(attacker PRIVATE shared)
target_compile_features(attacker PRIVATE cxx_std_20)
