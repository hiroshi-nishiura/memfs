cmake_minimum_required( VERSION 3.0 FATAL_ERROR )
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 11)
project(sample)

file(GLOB srcs *.o *.c *.cpp)

add_executable(sample ${srcs})
target_link_libraries(sample)
