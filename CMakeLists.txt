cmake_minimum_required(VERSION 3.22.1)
project(OptimalPathClient)

set(CMAKE_CXX_STANDARD 20)

add_executable(OptimalPathClient main.cpp Client.h Client.cpp fpng.h fpng.cpp stb_image_write.h)
target_compile_options(OptimalPathClient PRIVATE -O3 -march=native)
target_compile_definitions(OptimalPathClient PRIVATE STB_IMAGE_WRITE_IMPLEMENTATION)