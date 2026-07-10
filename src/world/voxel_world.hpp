#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_map>
#include <unordered_set>
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

struct GeneratedChunkData {
    ChunkCoord coord;
    std::array<BlockId, kChunkVolume> blocks = {};
};

class VoxelWorld {
public:
    VoxelWorld();
    ~VoxelWorld();

    VoxelWorld(VoxelWorld const&) = delete;
    auto operator=(VoxelWorld const&) -> VoxelWorld& = delete;
    VoxelWorld(VoxelWorld&&) = delete;
    auto operator=(VoxelWorld&&) -> VoxelWorld& = delete;

    void ensure_region(FVector3 const& player_position, int radius_chunks);
    auto visible_chunks(FVector3 const& player_position, int radius_chunks) -> std::vector<ChunkCoord>;
    auto get_block(FVector3I const& world_position) const -> BlockId;
    auto chunk_at(ChunkCoord const& coord) -> Chunk*;
    auto chunk_at(ChunkCoord const& coord) const -> Chunk const*;
    void shutdown_gpu();
    void shutdown_generation();

private:
    void schedule_chunk(ChunkCoord const& coord);
    void process_generated(std::size_t max_chunks_per_frame);
    void worker_main(std::stop_token stop_token);

    std::unordered_map<ChunkCoord, Chunk, ChunkCoordHasher> chunks_;
    std::unordered_set<ChunkCoord, ChunkCoordHasher> in_flight_chunks_;

    std::queue<ChunkCoord> pending_queue_;
    std::queue<GeneratedChunkData> completed_queue_;

    std::mutex queue_mutex_;
    std::condition_variable queue_condition_;
    std::vector<std::jthread> workers_;
    bool generation_stopped_ = false;
};
}
