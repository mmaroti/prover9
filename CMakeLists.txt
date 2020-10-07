cmake_minimum_required(VERSION 3.1)
project(prover9 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -O2)

add_subdirectory(ladr)
add_subdirectory(provers)
add_subdirectory(mace4)
