#pragma once

#include "world/voxel_world.hpp"

namespace voxel {
class ChunkMesher {
public:
    static void rebuild_mesh(VoxelWorld const& world, ChunkCoord const& coord, Chunk& chunk);
};
}
