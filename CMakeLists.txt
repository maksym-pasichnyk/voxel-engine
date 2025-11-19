cmake_minimum_required(VERSION 4.1)
project(VoxelEngine)

set(CMAKE_CXX_STANDARD 26)

find_package(GLEW REQUIRED)

add_subdirectory(deps/mathematics)
add_subdirectory(deps/SDL-release-3.4.0)

add_executable(VoxelEngine
    src/main.cpp
    src/core/application.cpp
    src/platform/window.cpp
    src/scene/fps_camera.cpp
    src/game/player_controller.cpp
    src/world/voxel_world.cpp
    src/world/world_generator.cpp
    src/world/chunk_mesher.cpp
    src/render/shader_program.cpp
    src/render/voxel_renderer.cpp
)

target_include_directories(VoxelEngine PRIVATE src)

target_link_libraries(VoxelEngine PRIVATE
    SDL3::SDL3
    GLEW::GLEW
    mathematics::mathematics
)
