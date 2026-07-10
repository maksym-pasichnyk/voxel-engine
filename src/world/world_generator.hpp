#pragma once

#include <array>

#include "world/voxel_world.hpp"

namespace voxel {
class WorldGenerator {
public:
    static auto generate_blocks(ChunkCoord const& coord) -> std::array<BlockId, kChunkVolume>;
};
}
