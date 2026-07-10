#include "world/voxel_world.hpp"

#include <algorithm>
#include <utility>

#include "Mathematics.hpp"
#include "world/chunk_mesher.hpp"
#include "world/world_generator.hpp"

namespace voxel {
VoxelWorld::VoxelWorld() {
    unsigned int const hardware_threads = std::thread::hardware_concurrency();
    unsigned int const worker_count = std::max(1u, hardware_threads > 1u ? hardware_threads - 1u : 1u);
    workers_.reserve(worker_count);
    for (unsigned int index = 0; index < worker_count; index += 1) {
        workers_.emplace_back([this](std::stop_token stop_token) {
            worker_main(stop_token);
        });
    }
}

VoxelWorld::~VoxelWorld() {
    shutdown_generation();
}

void VoxelWorld::worker_main(std::stop_token stop_token) {
    while (!stop_token.stop_requested()) {
        ChunkCoord coord;
        {
            std::unique_lock lock(queue_mutex_);
            queue_condition_.wait(lock, [&] {
                return generation_stopped_ || !pending_queue_.empty() || stop_token.stop_requested();
            });

            if (generation_stopped_ || stop_token.stop_requested()) {
                return;
            }

            coord = pending_queue_.front();
            pending_queue_.pop();
        }

        GeneratedChunkData generated_chunk;
        generated_chunk.coord = coord;
        generated_chunk.blocks = WorldGenerator::generate_blocks(coord);

        {
            std::scoped_lock lock(queue_mutex_);
            completed_queue_.push(std::move(generated_chunk));
        }
    }
}

void VoxelWorld::schedule_chunk(ChunkCoord const& coord) {
    auto [it, inserted] = chunks_.try_emplace(coord);
    if (!inserted && it->second.generated) {
        return;
    }

    {
        std::scoped_lock lock(queue_mutex_);
        if (generation_stopped_ || in_flight_chunks_.contains(coord)) {
            return;
        }

        in_flight_chunks_.insert(coord);
        pending_queue_.push(coord);
    }

    queue_condition_.notify_one();
}

void VoxelWorld::process_generated(std::size_t max_chunks_per_frame) {
    std::vector<GeneratedChunkData> completed_chunks;
    completed_chunks.reserve(max_chunks_per_frame);

    {
        std::scoped_lock lock(queue_mutex_);
        while (!completed_queue_.empty() && completed_chunks.size() < max_chunks_per_frame) {
            completed_chunks.emplace_back(std::move(completed_queue_.front()));
            completed_queue_.pop();
        }
    }

    for (GeneratedChunkData& generated_chunk : completed_chunks) {
        Chunk& chunk = chunks_[generated_chunk.coord];
        chunk.blocks = std::move(generated_chunk.blocks);
        chunk.generated = true;
        chunk.dirty = true;
        ChunkMesher::rebuild_mesh(*this, generated_chunk.coord, chunk);

        std::scoped_lock lock(queue_mutex_);
        in_flight_chunks_.erase(generated_chunk.coord);
    }
}

void VoxelWorld::ensure_region(FVector3 const& player_position, int radius_chunks) {
    process_generated(8);

    FVector3I const player_block = math::ToIntVector(player_position.Floor());
    ChunkCoord const center = world_to_chunk(player_block);

    for (int z = center.z - radius_chunks; z <= center.z + radius_chunks; z += 1) {
        for (int x = center.x - radius_chunks; x <= center.x + radius_chunks; x += 1) {
            for (int y = -1; y <= 3; y += 1) {
                schedule_chunk(ChunkCoord{.x = x, .y = y, .z = z});
            }
        }
    }
}

auto VoxelWorld::visible_chunks(FVector3 const& player_position, int radius_chunks) -> std::vector<ChunkCoord> {
    process_generated(16);

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

void VoxelWorld::shutdown_generation() {
    {
        std::scoped_lock lock(queue_mutex_);
        if (generation_stopped_) {
            return;
        }
        generation_stopped_ = true;
    }

    queue_condition_.notify_all();
    workers_.clear();
}
}
