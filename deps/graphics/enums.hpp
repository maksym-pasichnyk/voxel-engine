//
// Created by Maksym Pasichnyk on 24.03.2025.
//
#pragma once

namespace gfx {
    enum class ShaderStage {
        fragment,
        vertex
    };

    enum class BufferUsage {
        elements,
        vertices,
        uniform
    };

    enum class IndexType {
        uint16,
        uint32,
    };

    enum class Topology {
        points,
        line_strip,
        line_loop,
        lines,
        triangle_strip,
        triangle_fan,
        triangles,
        lines_adjacency,
        line_strip_adjacency,
        triangles_adjacency,
        triangle_strip_adjacency,
        patches,
    };

    enum class BlendFactor {
        zero,
        one,
        src_color,
        one_minus_src_color,
        dst_color,
        one_minus_dst_color,
        src_alpha,
        one_minus_src_alpha,
        dst_alpha,
        one_minus_dst_alpha,
    };

    enum class BlendOp {
        add,
        subtract,
        reverse_subtract,
        min,
        max,
    };

    enum class Format {
        r8g8_unorm,
        r8g8b8_unorm,
        r8g8b8a8_unorm,
        r32g32_sfloat,
        r32g32b32_sfloat,
        r32g32b32a32_sfloat,
    };

    enum class CullMode {
        none,
        front,
        back
    };

    enum class CompareOp {
        never,
        less,
        equal,
        less_or_equal,
        greater,
        not_equal,
        greater_or_equal,
        always
    };

    enum class StencilOp {
        keep,
        zero,
        replace,
        increment_and_clamp,
        decrement_and_clamp,
        invert,
        increment_and_wrap,
        decrement_and_wrap
    };

    enum class StencilFace {
        front,
        back,
        front_and_back
    };

    enum class VertexInputRate {
        vertex,
        instance
    };
}