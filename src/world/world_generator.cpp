#include "world/world_generator.hpp"

#include <cmath>

namespace voxel {
namespace {
auto terrain_height(int world_x, int world_z) -> int {
    float const hills = std::sin(static_cast<float>(world_x) * 0.08f) * 5.0f;
    float const ridges = std::cos(static_cast<float>(world_z) * 0.06f) * 7.0f;
    return 20 + static_cast<int>(hills + ridges);
}
}

void WorldGenerator::generate(ChunkCoord const& coord, Chunk& chunk) {
    FVector3I const origin = chunk_origin(coord);

    for (int z = 0; z < kChunkSize; z += 1) {
        for (int y = 0; y < kChunkSize; y += 1) {
            for (int x = 0; x < kChunkSize; x += 1) {
                int const world_x = origin.x + x;
                int const world_y = origin.y + y;
                int const world_z = origin.z + z;
                int const height = terrain_height(world_x, world_z);

                BlockId block = BlockId::air;
                if (world_y == height) {
                    block = BlockId::grass;
                } else if (world_y > height - 3 && world_y < height) {
                    block = BlockId::dirt;
                } else if (world_y < height) {
                    block = BlockId::stone;
                }

                chunk.blocks[flatten_index(x, y, z)] = block;
            }
        }
    }

    chunk.generated = true;
    chunk.dirty = true;
}
}
