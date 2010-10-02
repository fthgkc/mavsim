project(oooark)
cmake_minimum_required(VERSION 2.6)

enable_language(C CXX Fortran)

add_library(fAndC SHARED hello.f90 world.cpp)
target_link_libraries(fAndC gfortran)
add_executable(helloWorld test.cpp)
target_link_libraries(helloWorld fAndC)
link_directories(${oooark_BINARY_DIR})
