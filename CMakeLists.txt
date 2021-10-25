cmake_minimum_required(VERSION 3.15)
project(cilk_quicksort)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fopenmp" )
add_executable(quicksort
        src/main.cpp
        include/quicksort.h
        src/quicksort.cpp
        include/parallel_utils.h
        )
target_include_directories(quicksort PRIVATE include/)
