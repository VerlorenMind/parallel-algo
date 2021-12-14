cmake_minimum_required(VERSION 3.15)
project(quicksort)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-fopenmp" )

# set(CMAKE_PREFIX_PATH /usr/local/gcc11)
# set(CMAKE_C_COMPILER /usr/local/gcc11/bin/gcc)
# set(CMAKE_CXX_COMPILER /usr/local/gcc11/bin/g++)

add_executable(quicksort
        include/quicksort.h
        src/quicksort.cpp
        include/parallel_utils.h
        )
target_include_directories(quicksort PRIVATE include/)

add_executable(bfs src/bfs.cpp include/bfs.h src/bfs_benchmark.cpp)
target_include_directories(bfs PRIVATE include/)

add_executable(gen_cube src/gen_cube.cpp)
target_include_directories(gen_cube PRIVATE include/)
