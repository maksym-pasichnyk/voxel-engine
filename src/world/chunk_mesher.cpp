#include "world/chunk_mesher.hpp"

#include <array>

#include "Mathematics.hpp"

namespace voxel {
namespace {
struct FaceDefinition {
    FVector3I offset;
    std::array<FVector3, 6> corners;
    FVector3 normal;
};

constexpr std::array<FaceDefinition, 6> kFaces = {{
    {FVector3I(0, 0, 1), {FVector3(0, 0, 1), FVector3(1, 0, 1), FVector3(1, 1, 1), FVector3(0, 0, 1), FVector3(1, 1, 1), FVector3(0, 1, 1)}, FVector3(0, 0, 1)},
    {FVector3I(0, 0, -1), {FVector3(1, 0, 0), FVector3(0, 0, 0), FVector3(0, 1, 0), FVector3(1, 0, 0), FVector3(0, 1, 0), FVector3(1, 1, 0)}, FVector3(0, 0, -1)},
    {FVector3I(1, 0, 0), {FVector3(1, 0, 1), FVector3(1, 0, 0), FVector3(1, 1, 0), FVector3(1, 0, 1), FVector3(1, 1, 0), FVector3(1, 1, 1)}, FVector3(1, 0, 0)},
    {FVector3I(-1, 0, 0), {FVector3(0, 0, 0), FVector3(0, 0, 1), FVector3(0, 1, 1), FVector3(0, 0, 0), FVector3(0, 1, 1), FVector3(0, 1, 0)}, FVector3(-1, 0, 0)},
    {FVector3I(0, 1, 0), {FVector3(0, 1, 1), FVector3(1, 1, 1), FVector3(1, 1, 0), FVector3(0, 1, 1), FVector3(1, 1, 0), FVector3(0, 1, 0)}, FVector3(0, 1, 0)},
    {FVector3I(0, -1, 0), {FVector3(0, 0, 0), FVector3(1, 0, 0), FVector3(1, 0, 1), FVector3(0, 0, 0), FVector3(1, 0, 1), FVector3(0, 0, 1)}, FVector3(0, -1, 0)},
}};

auto block_color(BlockId block) -> FVector3 {
    switch (block) {
        case BlockId::grass:
            return FVector3(0.35f, 0.72f, 0.28f);
        case BlockId::dirt:
            return FVector3(0.49f, 0.34f, 0.21f);
        case BlockId::stone:
            return FVector3(0.55f, 0.57f, 0.60f);
        case BlockId::air:
            break;
    }
    return FVector3(0.0f, 0.0f, 0.0f);
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

                FVector3 const color = block_color(block);
                FVector3I const world_block = origin + FVector3I(x, y, z);

                for (FaceDefinition const& face : kFaces) {
                    if (world.get_block(world_block + face.offset) != BlockId::air) {
                        continue;
                    }

                    float const shade = face_shade(face.normal);
                    FVector3 const shaded_color = color * shade;
                    for (FVector3 const& corner : face.corners) {
                        FVector3 const position = math::ToFloatVector(world_block) + corner;
                        chunk.mesh_vertices.emplace_back(position.x);
                        chunk.mesh_vertices.emplace_back(position.y);
                        chunk.mesh_vertices.emplace_back(position.z);
                        chunk.mesh_vertices.emplace_back(shaded_color.x);
                        chunk.mesh_vertices.emplace_back(shaded_color.y);
                        chunk.mesh_vertices.emplace_back(shaded_color.z);
                    }
                }
            }
        }
    }

    chunk.mesh.vertex_count = static_cast<int>(chunk.mesh_vertices.size() / 6);
    chunk.dirty = false;
    chunk.mesh.uploaded = false;
}
}
