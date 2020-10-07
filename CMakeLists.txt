cmake_minimum_required(VERSION 3.8)
project(ladr LANGUAGES C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -O2)

add_subdirectory(ladr)
add_subdirectory(provers)
add_subdirectory(mace4)
