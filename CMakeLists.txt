cmake_minimum_required(VERSION 3.15)
project(quicksort)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fopenmp" )
add_executable(quicksort
        include/quicksort.h
        src/quicksort.cpp
        include/parallel_utils.h
        )
target_include_directories(quicksort PRIVATE include/)

add_executable(bfs src/bfs.cpp include/bfs.h)

add_executable(gen_cube src/gen_cube.cpp)