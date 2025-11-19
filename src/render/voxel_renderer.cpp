#include "render/voxel_renderer.hpp"

#include <cstring>

#include "Mathematics.hpp"

namespace render {
namespace {
constexpr char const* kVertexShader = R"(
#version 410 core
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

uniform mat4 u_view_projection;

out vec3 v_color;

void main() {
    v_color = in_color;
    gl_Position = u_view_projection * vec4(in_position, 1.0);
}
)";

constexpr char const* kFragmentShader = R"(
#version 410 core
in vec3 v_color;
out vec4 out_color;

void main() {
    out_color = vec4(v_color, 1.0);
}
)";
}

auto VoxelRenderer::initialize() -> bool {
    if (!program_.create(kVertexShader, kFragmentShader)) {
        return false;
    }

    view_projection_location_ = glGetUniformLocation(program_.id(), "u_view_projection");
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // The camera/projection math uses a left-handed view space, so visible faces
    // arrive clockwise after projection unless we flip OpenGL's front-face rule.
    glFrontFace(GL_CW);
    return true;
}

void VoxelRenderer::upload_chunk_mesh(voxel::Chunk& chunk) {
    if (chunk.mesh.uploaded) {
        return;
    }

    if (chunk.mesh.vao == 0) {
        glGenVertexArrays(1, &chunk.mesh.vao);
    }
    if (chunk.mesh.vbo == 0) {
        glGenBuffers(1, &chunk.mesh.vbo);
    }

    glBindVertexArray(chunk.mesh.vao);
    glBindBuffer(GL_ARRAY_BUFFER, chunk.mesh.vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(sizeof(float) * chunk.mesh_vertices.size()),
        chunk.mesh_vertices.data(),
        GL_STATIC_DRAW
    );

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(3 * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    chunk.mesh.uploaded = true;
}

void VoxelRenderer::render(voxel::VoxelWorld& world, scene::FpsCamera const& camera, int width, int height) {
    glViewport(0, 0, width, height);
    glClearColor(0.78f, 0.88f, 0.98f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    FMatrix4x4 const projection = math::CreatePerspectiveFieldOfView(
        math::DegreesToRadians(70.0f),
        static_cast<float>(width) / static_cast<float>(height),
        0.1f,
        512.0f
    );
    FMatrix4x4 const view_projection = projection * camera.view_matrix();

    program_.use();
    glUniformMatrix4fv(view_projection_location_, 1, GL_FALSE, &view_projection.Columns[0].x);

    for (voxel::ChunkCoord const& coord : world.visible_chunks(camera.position(), 10)) {
        voxel::Chunk* chunk = world.chunk_at(coord);
        if (chunk == nullptr || chunk->mesh.vertex_count == 0) {
            continue;
        }

        upload_chunk_mesh(*chunk);
        glBindVertexArray(chunk->mesh.vao);
        glDrawArrays(GL_TRIANGLES, 0, chunk->mesh.vertex_count);
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

void VoxelRenderer::shutdown(voxel::VoxelWorld& world) {
    world.shutdown_gpu();
    program_.destroy();
}
}
