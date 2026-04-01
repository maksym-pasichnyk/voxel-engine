#include "render/voxel_renderer.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <optional>
#include <print>
#include <string>
#include <vector>

#include "Mathematics.hpp"
#include "nlohmann/json.hpp"
#include "stb_image.h"

namespace render {
namespace {
constexpr char const* kVertexShader = R"(
#version 410 core
layout(location = 0) in vec3 in_position;
layout(location = 1) in vec2 in_uv;
layout(location = 2) in float in_texture_index;
layout(location = 3) in float in_shade;

uniform mat4 u_view_projection;

out vec2 v_uv;
flat out float v_texture_index;
out float v_shade;

void main() {
    v_uv = in_uv;
    v_texture_index = in_texture_index;
    v_shade = in_shade;
    gl_Position = u_view_projection * vec4(in_position, 1.0);
}
)";

constexpr char const* kFragmentShader = R"(
#version 410 core
in vec2 v_uv;
flat in float v_texture_index;
in float v_shade;

uniform sampler2DArray u_block_textures;

out vec4 out_color;

void main() {
    vec4 albedo = texture(u_block_textures, vec3(v_uv, v_texture_index));
    out_color = vec4(albedo.rgb * v_shade, albedo.a);
}
)";

struct LoadedTexture {
    int width = 0;
    int height = 0;
    std::vector<unsigned char> pixels;
};

constexpr std::array<char const*, 3> kTextureAssets = {
    "assets/textures/grass.json",
    "assets/textures/dirt.json",
    "assets/textures/stone.json",
};

auto resolve_asset_path(std::filesystem::path const& relative_path) -> std::optional<std::filesystem::path> {
    if (std::filesystem::exists(relative_path)) {
        return std::filesystem::weakly_canonical(relative_path);
    }

    return std::nullopt;
}

auto load_texture_description(std::filesystem::path const& description_path) -> std::optional<std::filesystem::path> {
    std::ifstream stream(description_path);
    if (!stream.is_open()) {
        std::println(stderr, "Failed to open texture description: {}", description_path.string());
        return std::nullopt;
    }

    nlohmann::json document;
    stream >> document;
    if (!document.contains("image") || !document["image"].is_string()) {
        std::println(stderr, "Texture description is missing image path: {}", description_path.string());
        return std::nullopt;
    }

    std::filesystem::path const image_path = document["image"].get<std::string>();
    return resolve_asset_path(image_path);
}

auto load_texture_image(std::filesystem::path const& description_path) -> std::optional<LoadedTexture> {
    std::optional<std::filesystem::path> const image_path = load_texture_description(description_path);
    if (!image_path.has_value()) {
        return std::nullopt;
    }

    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_uc* pixels = stbi_load(image_path->string().c_str(), &width, &height, &channels, 4);
    if (pixels == nullptr) {
        std::println(stderr, "Failed to load texture image: {}", image_path->string());
        return std::nullopt;
    }

    LoadedTexture texture;
    texture.width = width;
    texture.height = height;
    texture.pixels.assign(pixels, pixels + (width * height * 4));
    stbi_image_free(pixels);
    return texture;
}

auto create_block_texture_array(GLuint& texture_array) -> bool {
    std::vector<LoadedTexture> textures;
    textures.reserve(kTextureAssets.size());

    stbi_set_flip_vertically_on_load(1);
    for (char const* texture_asset : kTextureAssets) {
        std::optional<std::filesystem::path> const description_path = resolve_asset_path(texture_asset);
        if (!description_path.has_value()) {
            std::println(stderr, "Failed to resolve texture asset: {}", texture_asset);
            return false;
        }

        std::optional<LoadedTexture> const texture = load_texture_image(*description_path);
        if (!texture.has_value()) {
            return false;
        }

        textures.emplace_back(std::move(*texture));
    }

    if (textures.empty()) {
        std::println(stderr, "No block textures were loaded.");
        return false;
    }

    int const width = textures.front().width;
    int const height = textures.front().height;
    for (LoadedTexture const& texture : textures) {
        if (texture.width != width || texture.height != height) {
            std::println(stderr, "Block textures must all share the same size.");
            return false;
        }
    }

    glGenTextures(1, &texture_array);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, 0);
    glTexImage3D(
        GL_TEXTURE_2D_ARRAY,
        0,
        GL_RGBA8,
        width,
        height,
        static_cast<GLsizei>(textures.size()),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        nullptr
    );

    for (std::size_t index = 0; index < textures.size(); index += 1) {
        LoadedTexture const& texture = textures[index];
        glTexSubImage3D(
            GL_TEXTURE_2D_ARRAY,
            0,
            0,
            0,
            static_cast<GLint>(index),
            texture.width,
            texture.height,
            1,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            texture.pixels.data()
        );
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    return true;
}
}

auto VoxelRenderer::initialize() -> bool {
    if (!program_.create(kVertexShader, kFragmentShader)) {
        return false;
    }
    if (!create_block_texture_array(block_texture_array_)) {
        program_.destroy();
        return false;
    }

    view_projection_location_ = glGetUniformLocation(program_.id(), "u_view_projection");
    block_texture_location_ = glGetUniformLocation(program_.id(), "u_block_textures");
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 7 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(5 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 7 * static_cast<GLsizei>(sizeof(float)), reinterpret_cast<void*>(6 * sizeof(float)));

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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D_ARRAY, block_texture_array_);
    glUniform1i(block_texture_location_, 0);

    for (voxel::ChunkCoord const& coord : world.visible_chunks(camera.position(), 10)) {
        voxel::Chunk* chunk = world.chunk_at(coord);
        if (chunk == nullptr || chunk->mesh.vertex_count == 0) {
            continue;
        }

        upload_chunk_mesh(*chunk);
        glBindVertexArray(chunk->mesh.vao);
        glDrawArrays(GL_TRIANGLES, 0, chunk->mesh.vertex_count);
    }

    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void VoxelRenderer::shutdown(voxel::VoxelWorld& world) {
    world.shutdown_gpu();
    if (block_texture_array_ != 0) {
        glDeleteTextures(1, &block_texture_array_);
        block_texture_array_ = 0;
    }
    program_.destroy();
}
}
