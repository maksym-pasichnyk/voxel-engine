cmake_minimum_required(VERSION 4.1)
project(stb)

set(CMAKE_CXX_STANDARD 26)

add_library(stb_image STATIC src/stb_image.h src/stb_image.cpp)
target_include_directories(stb_image PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(stb::image ALIAS stb_image)