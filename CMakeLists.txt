cmake_minimum_required(VERSION 3.1)
project(prover9 LANGUAGES C)

set(CMAKE_C_STANDARD 99)

if(NOT EXISTS ${CMAKE_BINARY_DIR}/CMakeCache.txt)
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "" FORCE)
  endif()
endif()

if(MSVC)
  add_compile_options(/W3)
else()
  add_compile_options(-Wall)
endif()

add_subdirectory(ladr)
add_subdirectory(provers)
add_subdirectory(mace4)
