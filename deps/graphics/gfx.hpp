//
// Created by Maksym Pasichnyk on 03.12.2024.
//
#pragma once

#include <cmath>
#include <span>
#include <string>
#include <optional>

#include "enums.hpp"

namespace gfx {
    using Fence = struct Fence_T*;
    using Image = struct Image_T*;
    using Device = struct Device_T*;
    using Buffer = struct Buffer_T*;
    using Sampler = struct Sampler_T*;
    using Context = struct Context_T*;
    using ShaderModule = struct ShaderModule_T*;
    using ShaderObject = struct ShaderObject_T*;
    using CommandBuffer = struct CommandBuffer_T*;
    using RenderPipeline = struct RenderPipeline_T*;
    using RenderCommandEncoder = struct RenderCommandEncoder_T*;

    struct ShaderCreateInfo {
        std::span<char const> bytes;
    };

    struct ShaderObjectCreateInfo {
        ShaderModule module;
        ShaderStage  stage;
        std::string  entry_point;
    };

    struct BufferCreateInfo {
        BufferUsage usage;
        std::size_t size;
        std::uint32_t mode;
    };

    struct BufferAllocatorCreateInfo {
        BufferUsage usage;
        std::size_t page_size;
    };

    struct VertexBinding {
        std::size_t stride;
        VertexInputRate input_rate;
    };

    struct VertexAttribute {
        std::uint32_t binding;
        std::uint32_t location;
        Format format;
        std::size_t offset;
    };

    struct VertexInputStateCreateInfo {
        std::span<VertexBinding const> bindings = {};
        std::span<VertexAttribute const> attributes = {};
    };

    struct DepthStateCreateInfo {
        CompareOp depth_compare_op = {};
        bool is_depth_write_enabled = {};
    };

    struct BlendAttachmentState {
        bool        enable = {};
        BlendFactor src_color = {};
        BlendFactor dst_color = {};
        BlendOp     color_op = {};
        BlendFactor src_alpha = {};
        BlendFactor dst_alpha = {};
        BlendOp     alpha_op = {};
    };

    struct BlendStateCreateInfo {
        std::span<BlendAttachmentState const> attachments = {};
    };

    struct RenderPipelineCreateInfo {
        ShaderObject vertex_shader = {};
        ShaderObject fragment_shader = {};
        CullMode cull_mode = {};
        BlendStateCreateInfo blend_state = {};
        DepthStateCreateInfo depth_state = {};
        VertexInputStateCreateInfo vertex_input_state = {};
    };

    struct ColorClearValue {
        std::float_t r = {};
        std::float_t g = {};
        std::float_t b = {};
        std::float_t a = {};
    };

    struct ColorWriteMask {
        bool r = {};
        bool g = {};
        bool b = {};
        bool a = {};
    };

    struct RenderPassColorAttachmentInfo {
        ColorClearValue clear_value = {};
    };

    struct RenderPassDepthAttachmentInfo {
        std::float_t clear_value = {};
    };

    struct RenderPassStencilAttachmentInfo {
        std::int32_t clear_value = {};
    };

    struct RenderPassBeginInfo {
        std::uint32_t framebuffer = {};
        std::span<RenderPassColorAttachmentInfo const> color_attachments = {};
        std::optional<RenderPassDepthAttachmentInfo const> depth_attachment = {};
        std::optional<RenderPassStencilAttachmentInfo const> stencil_attachment = {};
    };

    struct Offset2D {
        std::int32_t x;
        std::int32_t y;
    };

    struct Offset3D {
        std::int32_t x;
        std::int32_t y;
        std::int32_t z;
    };

    struct Extent2D {
        std::uint32_t width;
        std::uint32_t height;
    };

    struct Extent3D {
        std::uint32_t width;
        std::uint32_t height;
        std::uint32_t depth;
    };

    struct Viewport {
        std::float_t x;
        std::float_t y;
        std::float_t width;
        std::float_t height;
    };

    struct Rect2D {
        std::int32_t x;
        std::int32_t y;
        std::uint32_t width;
        std::uint32_t height;
    };

    enum class ImageType {
        _1D,
        _2D,
        _3D,
    };

    struct ImageCreateInfo {
        ImageType type;
        Format format;
        Extent3D extent;
        std::uint32_t mip_levels;
        std::uint32_t array_layers;
    };

    struct MemoryToImageCopy {
        void const*     host_pointer;
        std::uint32_t   memory_row_length;
        std::uint32_t   memory_image_height;
        std::uint32_t   mip_level;
        std::uint32_t   base_array_layer;
        std::uint32_t   layer_count;
        Offset3D        image_offset;
        Extent3D        image_extent;
    };

    enum class Filter {
        nearest,
        linear,
    };

    enum class SamplerMipmapMode {
        nearest,
        linear,
    };

    enum class SamplerAddressMode {
        repeat,
        mirror_repeat,
        clamp_to_edge,
        clamp_to_border,
        mirror_clamp_to_edge
    };

    struct SamplerCreateInfo {
        Filter             mag_filter;
        Filter             min_filter;
        SamplerMipmapMode  mipmap_mode;
        SamplerAddressMode address_mode_u;
        SamplerAddressMode address_mode_v;
        SamplerAddressMode address_mode_w;
        std::float_t       mip_lod_bias;
        bool               anisotropy_enable;
        std::float_t       max_anisotropy;
        bool               compare_enable;
        CompareOp        compare_op;
        std::float_t       min_lod;
        std::float_t       max_lod;
    };

    auto GetProcAddress(Context context, char const* name) -> void*;
    auto CreateContext() -> Context;
    void DeleteContext(Context context);
    auto CreateDevice(Context context) -> Device;
    void DeleteDevice(Context context, Device device);
    auto CreateBuffer(Device device, BufferCreateInfo const& create_info) -> Buffer;
    void DeleteBuffer(Device device, Buffer buffer);
    void UpdateBuffer(Device device, Buffer buffer, std::size_t offset, std::size_t size, void const* data);
    auto CreateShaderModule(Device device, ShaderCreateInfo const& create_info) -> ShaderModule;
    void DeleteShaderModule(Device device, ShaderModule shader_module);
    auto CreateShaderObject(Device device, ShaderObjectCreateInfo const& create_info) -> ShaderObject;
    void DeleteShaderObject(Device device, ShaderObject shader_module);
    auto CreateRenderPipeline(Device device, RenderPipelineCreateInfo const& create_info) -> RenderPipeline;
    void DeleteRenderPipeline(Device device, RenderPipeline pipeline);
    auto CreateImage(Device device, ImageCreateInfo const& create_info) -> Image;
    void DeleteImage(Device device, Image image);
    void CopyMemoryToImage(Device device, Image image, std::span<MemoryToImageCopy const> regions);
    auto CreateSampler(Device device, SamplerCreateInfo const& create_info) -> Sampler;
    void DeleteSampler(Device device, Sampler sampler);
    void Submit(Device device, CommandBuffer command_buffer);
    auto CreateCommandBuffer(Device device) -> CommandBuffer;
    void DeleteCommandBuffer(Device device, CommandBuffer command_buffer);
    void ResetCommandBuffer(Device device, CommandBuffer command_buffer);
    void BeginRenderPass(CommandBuffer command_buffer, RenderPassBeginInfo const& begin_info);
    void EndRenderPass(CommandBuffer command_buffer);
    void SetScissor(CommandBuffer command_buffer, std::uint32_t first_index, Rect2D const scissor);
    void SetViewport(CommandBuffer command_buffer, std::uint32_t first_index, Viewport const viewport);
    void SetRenderPipeline(CommandBuffer command_buffer, RenderPipeline pipeline);
    void SetVertexBuffers(CommandBuffer command_buffer, std::span<Buffer const> buffers);
    void BindBuffer(CommandBuffer command_buffer, std::uint32_t binding, Buffer buffer);
    void BindCombinedImageSampler(CommandBuffer command_buffer, std::uint32_t binding, Image image, Sampler sampler);
    void SetBytes(CommandBuffer command_buffer, std::uint32_t binding, std::uint32_t size, void const* data);
    void Draw(CommandBuffer command_buffer, Topology topology, std::uint32_t vertex_count, std::uint32_t first_vertex, std::uint32_t instance_count);
    void DrawIndexed(CommandBuffer command_buffer, Topology topology, Buffer index_buffer, std::uint32_t index_offset, std::uint32_t index_count, std::uint32_t base_vertex, IndexType index_type, std::uint32_t instance_count);
    void SetStencilTestEnable(CommandBuffer command_buffer, bool stencil_test_enable);
    void SetStencilWriteMask(CommandBuffer command_buffer, StencilFace face_mask, std::uint32_t write_mask);
    void SetStencilCompareMask(CommandBuffer command_buffer, StencilFace face_mask, std::uint32_t compare_mask);
    void SetStencilReference(CommandBuffer command_buffer, StencilFace face_mask, std::uint32_t reference);
    void SetStencilOp(CommandBuffer command_buffer, StencilFace face_mask, StencilOp fail_op, StencilOp pass_op, StencilOp depth_fail_op, CompareOp compare_op);
    void SetColorWriteMask(CommandBuffer command_buffer, ColorWriteMask color_write_mask);
    void ClearStencil(CommandBuffer command_buffer, std::int32_t clear_value);
}
