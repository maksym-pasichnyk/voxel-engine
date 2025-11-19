#pragma once

#include "world/voxel_world.hpp"

namespace voxel {
class WorldGenerator {
public:
    static void generate(ChunkCoord const& coord, Chunk& chunk);
};
}
