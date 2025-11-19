#pragma once

#include <array>
#include <cstdint>

#include "FVector3I.hpp"

namespace voxel {
constexpr int kChunkSize = 16;
constexpr int kChunkVolume = kChunkSize * kChunkSize * kChunkSize;

enum class BlockId : std::uint8_t {
    air = 0,
    grass,
    dirt,
    stone,
};

struct ChunkCoord {
    int x = 0;
    int y = 0;
    int z = 0;

    auto operator==(ChunkCoord const&) const -> bool = default;
};

struct ChunkCoordHasher {
    auto operator()(ChunkCoord const& coord) const -> std::size_t {
        std::size_t seed = 0;
        seed ^= static_cast<std::size_t>(coord.x) * 73856093u;
        seed ^= static_cast<std::size_t>(coord.y) * 19349663u;
        seed ^= static_cast<std::size_t>(coord.z) * 83492791u;
        return seed;
    }
};

inline auto floor_div(int value, int divisor) -> int {
    int quotient = value / divisor;
    int remainder = value % divisor;
    if (remainder != 0 && ((remainder < 0) != (divisor < 0))) {
        quotient -= 1;
    }
    return quotient;
}

inline auto positive_mod(int value, int divisor) -> int {
    int remainder = value % divisor;
    return remainder < 0 ? remainder + divisor : remainder;
}

inline auto world_to_chunk(FVector3I const& world) -> ChunkCoord {
    return ChunkCoord{
        .x = floor_div(world.x, kChunkSize),
        .y = floor_div(world.y, kChunkSize),
        .z = floor_div(world.z, kChunkSize),
    };
}

inline auto local_from_world(FVector3I const& world) -> FVector3I {
    return FVector3I(
        positive_mod(world.x, kChunkSize),
        positive_mod(world.y, kChunkSize),
        positive_mod(world.z, kChunkSize)
    );
}

inline auto chunk_origin(ChunkCoord const& coord) -> FVector3I {
    return FVector3I(coord.x * kChunkSize, coord.y * kChunkSize, coord.z * kChunkSize);
}

inline auto flatten_index(int x, int y, int z) -> int {
    return x + y * kChunkSize + z * kChunkSize * kChunkSize;
}

inline auto flatten_index(FVector3I const& local) -> int {
    return flatten_index(local.x, local.y, local.z);
}
}
