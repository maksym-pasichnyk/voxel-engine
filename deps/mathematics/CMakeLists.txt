cmake_minimum_required(VERSION 4.1)
project(mathematics)

set(CMAKE_CXX_STANDARD 26)

add_library(mathematics INTERFACE src/FLWCVector2.hpp src/FLWCVector3.hpp src/FLWCVector4.hpp src/FMatrix3x3.hpp src/FMatrix4x4.hpp src/FVector2.hpp src/FVector2D.hpp src/FVector2I.hpp src/FVector3.hpp src/FVector3D.hpp src/FVector3I.hpp src/FVector4.hpp src/FVector4D.hpp src/FVector4I.hpp src/Mathematics.hpp)
target_include_directories(mathematics INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_library(mathematics::mathematics ALIAS mathematics)