#include "world/chunk_mesher.hpp"

#include <array>

#include "Mathematics.hpp"

namespace voxel {
namespace {
struct TextureUv {
    float u;
    float v;
};

struct FaceDefinition {
    FVector3I offset;
    std::array<FVector3, 6> corners;
    std::array<TextureUv, 6> uvs;
    FVector3 normal;
};

constexpr std::array<FaceDefinition, 6> kFaces = {{
    {FVector3I(0, 0, 1), {FVector3(0, 0, 1), FVector3(1, 0, 1), FVector3(1, 1, 1), FVector3(0, 0, 1), FVector3(1, 1, 1), FVector3(0, 1, 1)}, {TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 1.0f}}, FVector3(0, 0, 1)},
    {FVector3I(0, 0, -1), {FVector3(1, 0, 0), FVector3(0, 0, 0), FVector3(0, 1, 0), FVector3(1, 0, 0), FVector3(0, 1, 0), FVector3(1, 1, 0)}, {TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 1.0f}}, FVector3(0, 0, -1)},
    {FVector3I(1, 0, 0), {FVector3(1, 0, 1), FVector3(1, 0, 0), FVector3(1, 1, 0), FVector3(1, 0, 1), FVector3(1, 1, 0), FVector3(1, 1, 1)}, {TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 1.0f}}, FVector3(1, 0, 0)},
    {FVector3I(-1, 0, 0), {FVector3(0, 0, 0), FVector3(0, 0, 1), FVector3(0, 1, 1), FVector3(0, 0, 0), FVector3(0, 1, 1), FVector3(0, 1, 0)}, {TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 1.0f}}, FVector3(-1, 0, 0)},
    {FVector3I(0, 1, 0), {FVector3(0, 1, 1), FVector3(1, 1, 1), FVector3(1, 1, 0), FVector3(0, 1, 1), FVector3(1, 1, 0), FVector3(0, 1, 0)}, {TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 1.0f}}, FVector3(0, 1, 0)},
    {FVector3I(0, -1, 0), {FVector3(0, 0, 0), FVector3(1, 0, 0), FVector3(1, 0, 1), FVector3(0, 0, 0), FVector3(1, 0, 1), FVector3(0, 0, 1)}, {TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 0.0f}, TextureUv{1.0f, 1.0f}, TextureUv{0.0f, 1.0f}}, FVector3(0, -1, 0)},
}};

auto block_texture_layer(BlockId block) -> float {
    switch (block) {
        case BlockId::grass:
            return 0.0f;
        case BlockId::dirt:
            return 1.0f;
        case BlockId::stone:
            return 2.0f;
        case BlockId::air:
            break;
    }
    return 0.0f;
}

auto face_shade(FVector3 const& normal) -> float {
    FVector3 const light = FVector3(0.5f, 1.0f, 0.4f).Normalized();
    return std::max(0.25f, normal.Dot(light));
}
}

void ChunkMesher::rebuild_mesh(VoxelWorld const& world, ChunkCoord const& coord, Chunk& chunk) {
    chunk.mesh_vertices.clear();
    FVector3I const origin = chunk_origin(coord);

    for (int z = 0; z < kChunkSize; z += 1) {
        for (int y = 0; y < kChunkSize; y += 1) {
            for (int x = 0; x < kChunkSize; x += 1) {
                BlockId const block = chunk.blocks[flatten_index(x, y, z)];
                if (block == BlockId::air) {
                    continue;
                }

                float const texture_layer = block_texture_layer(block);
                FVector3I const world_block = origin + FVector3I(x, y, z);

                for (FaceDefinition const& face : kFaces) {
                    if (world.get_block(world_block + face.offset) != BlockId::air) {
                        continue;
                    }

                    float const shade = face_shade(face.normal);
                    for (std::size_t vertex_index = 0; vertex_index < face.corners.size(); vertex_index += 1) {
                        FVector3 const& corner = face.corners[vertex_index];
                        TextureUv const uv = face.uvs[vertex_index];
                        FVector3 const position = math::ToFloatVector(world_block) + corner;
                        chunk.mesh_vertices.emplace_back(position.x);
                        chunk.mesh_vertices.emplace_back(position.y);
                        chunk.mesh_vertices.emplace_back(position.z);
                        chunk.mesh_vertices.emplace_back(uv.u);
                        chunk.mesh_vertices.emplace_back(uv.v);
                        chunk.mesh_vertices.emplace_back(texture_layer);
                        chunk.mesh_vertices.emplace_back(shade);
                    }
                }
            }
        }
    }

    chunk.mesh.vertex_count = static_cast<int>(chunk.mesh_vertices.size() / 7);
    chunk.dirty = false;
    chunk.mesh.uploaded = false;
}
}
