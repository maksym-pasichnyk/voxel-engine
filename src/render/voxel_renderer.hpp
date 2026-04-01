#pragma once

#include "render/shader_program.hpp"
#include "scene/fps_camera.hpp"
#include "world/voxel_world.hpp"

namespace render {
class VoxelRenderer {
public:
    auto initialize() -> bool;
    void render(voxel::VoxelWorld& world, scene::FpsCamera const& camera, int width, int height);
    void shutdown(voxel::VoxelWorld& world);

private:
    void upload_chunk_mesh(voxel::Chunk& chunk);

    ShaderProgram program_;
    GLint view_projection_location_ = -1;
    GLint block_texture_location_ = -1;
    GLuint block_texture_array_ = 0;
};
}
