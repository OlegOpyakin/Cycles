cmake_minimum_required(VERSION 3.6)
project(Cycles)

set(CMAKE_CXX_FLAGS "-std=c++20 -g -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++20 -Wall -g -O0 -fsanitize=address -fsanitize=undefined")

add_library(Graph src/Graph.cpp)
target_include_directories(Graph PUBLIC inc/)

add_executable(CycleDemo src/main.cpp)
target_link_libraries(CycleDemo Graph)
target_include_directories(CycleDemo PUBLIC inc/)
