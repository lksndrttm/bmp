cmake_minimum_required(VERSION 3.20)
project(BMP VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall)

add_executable(
    bmp
    include/bmp.h
    include/bmp_exceptions.h
    src/bmp.cpp
    src/main.cpp
)

target_include_directories(bmp PRIVATE include)
