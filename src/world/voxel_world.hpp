#pragma once

#include <unordered_map>
#include <vector>

#include <GL/glew.h>

#include "FVector3.hpp"
#include "world/voxel_types.hpp"

namespace voxel {
struct ChunkMesh {
    GLuint vao = 0;
    GLuint vbo = 0;
    int vertex_count = 0;
    bool uploaded = false;
};

struct Chunk {
    std::array<BlockId, kChunkVolume> blocks = {};
    bool generated = false;
    bool dirty = true;
    std::vector<float> mesh_vertices;
    ChunkMesh mesh = {};
};

class VoxelWorld {
public:
    VoxelWorld() = default;
    ~VoxelWorld() = default;

    void ensure_region(FVector3 const& player_position, int radius_chunks);
    auto visible_chunks(FVector3 const& player_position, int radius_chunks) -> std::vector<ChunkCoord>;
    auto get_block(FVector3I const& world_position) const -> BlockId;
    auto chunk_at(ChunkCoord const& coord) -> Chunk*;
    auto chunk_at(ChunkCoord const& coord) const -> Chunk const*;
    void shutdown_gpu();

private:
    auto ensure_chunk(ChunkCoord const& coord) -> Chunk&;

    std::unordered_map<ChunkCoord, Chunk, ChunkCoordHasher> chunks_;
};
}
