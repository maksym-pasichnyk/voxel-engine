#include "world/voxel_world.hpp"

#include <algorithm>

#include "Mathematics.hpp"
#include "world/chunk_mesher.hpp"
#include "world/world_generator.hpp"

namespace voxel {
auto VoxelWorld::ensure_chunk(ChunkCoord const& coord) -> Chunk& {
    auto [it, inserted] = chunks_.try_emplace(coord);
    Chunk& chunk = it->second;
    if (inserted || !chunk.generated) {
        WorldGenerator::generate(coord, chunk);
        ChunkMesher::rebuild_mesh(*this, coord, chunk);
    }
    return chunk;
}

void VoxelWorld::ensure_region(FVector3 const& player_position, int radius_chunks) {
    FVector3I const player_block = math::ToIntVector(player_position.Floor());
    ChunkCoord const center = world_to_chunk(player_block);

    for (int z = center.z - radius_chunks; z <= center.z + radius_chunks; z += 1) {
        for (int x = center.x - radius_chunks; x <= center.x + radius_chunks; x += 1) {
            for (int y = -1; y <= 3; y += 1) {
                ensure_chunk(ChunkCoord{.x = x, .y = y, .z = z});
            }
        }
    }
}

auto VoxelWorld::visible_chunks(FVector3 const& player_position, int radius_chunks) -> std::vector<ChunkCoord> {
    FVector3I const player_block = math::ToIntVector(player_position.Floor());
    ChunkCoord const center = world_to_chunk(player_block);
    std::vector<ChunkCoord> visible;

    for (auto const& [coord, chunk] : chunks_) {
        if (!chunk.generated) {
            continue;
        }

        int const dx = coord.x - center.x;
        int const dz = coord.z - center.z;
        if (dx * dx + dz * dz <= radius_chunks * radius_chunks) {
            visible.emplace_back(coord);
        }
    }

    std::ranges::sort(visible, [&](ChunkCoord const& lhs, ChunkCoord const& rhs) {
        int const ldx = lhs.x - center.x;
        int const ldz = lhs.z - center.z;
        int const rdx = rhs.x - center.x;
        int const rdz = rhs.z - center.z;
        return (ldx * ldx + ldz * ldz) < (rdx * rdx + rdz * rdz);
    });

    return visible;
}

auto VoxelWorld::get_block(FVector3I const& world_position) const -> BlockId {
    ChunkCoord const coord = world_to_chunk(world_position);
    FVector3I const local = local_from_world(world_position);
    if (auto it = chunks_.find(coord); it != chunks_.end() && it->second.generated) {
        return it->second.blocks[flatten_index(local)];
    }
    return BlockId::air;
}

auto VoxelWorld::chunk_at(ChunkCoord const& coord) -> Chunk* {
    if (auto it = chunks_.find(coord); it != chunks_.end()) {
        return &it->second;
    }
    return nullptr;
}

auto VoxelWorld::chunk_at(ChunkCoord const& coord) const -> Chunk const* {
    if (auto it = chunks_.find(coord); it != chunks_.end()) {
        return &it->second;
    }
    return nullptr;
}

void VoxelWorld::shutdown_gpu() {
    for (auto& [_, chunk] : chunks_) {
        if (chunk.mesh.vbo != 0) {
            glDeleteBuffers(1, &chunk.mesh.vbo);
            chunk.mesh.vbo = 0;
        }
        if (chunk.mesh.vao != 0) {
            glDeleteVertexArrays(1, &chunk.mesh.vao);
            chunk.mesh.vao = 0;
        }
    }
}
}
