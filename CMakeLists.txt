cmake_minimum_required(VERSION 3.6)
project(Cycles)

set(CMAKE_CXX_FLAGS "-std=c++20 -g -O3")
set(CMAKE_CXX_FLAGS_DEBUG "-std=c++20 -Wall -g -O0 -fsanitize=address -fsanitize=undefined")

add_library(Graph src/Graph.cc)
target_include_directories(Graph PUBLIC inc/)

add_executable(Cycles src/main.cc)
target_link_libraries(Cycles Graph)
target_include_directories(Cycles PUBLIC inc/)
