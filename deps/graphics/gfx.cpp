//
// Created by Maksym Pasichnyk on 17.07.2025.
//

#include "gfx.hpp"

#include <vector>
#include <memory>
// #include "arena.hpp"
// #include "stl/lib.hpp"

#include <dlfcn.h>
// #include <SDL_opengl.h>

#define SPV_ENABLE_UTILITY_CODE

#include "spirv_glsl.hpp"
#include "spirv_parser.hpp"

namespace {
    constexpr auto __to_gl_topology(gfx::Topology const topology) -> std::int32_t {
        switch (topology) {
            case gfx::Topology::points: return GL_POINTS;
            case gfx::Topology::line_strip: return GL_LINE_STRIP;
            case gfx::Topology::line_loop: return GL_LINE_LOOP;
            case gfx::Topology::lines: return GL_LINES;
            case gfx::Topology::triangle_strip: return GL_TRIANGLE_STRIP;
            case gfx::Topology::triangle_fan: return GL_TRIANGLE_FAN;
            case gfx::Topology::triangles: return GL_TRIANGLES;
            case gfx::Topology::lines_adjacency: return GL_LINES_ADJACENCY;
            case gfx::Topology::line_strip_adjacency: return GL_LINE_STRIP_ADJACENCY;
            case gfx::Topology::triangles_adjacency: return GL_TRIANGLES_ADJACENCY;
            case gfx::Topology::triangle_strip_adjacency: return GL_TRIANGLE_STRIP_ADJACENCY;
            case gfx::Topology::patches: return GL_PATCHES;
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_index_type(gfx::IndexType const index_type) -> std::int32_t {
        switch (index_type) {
            case gfx::IndexType::uint16: return GL_UNSIGNED_SHORT;
            case gfx::IndexType::uint32: return GL_UNSIGNED_INT;
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_shader_type(gfx::ShaderStage const shader_stage) -> std::int32_t {
        switch (shader_stage) {
            case gfx::ShaderStage::fragment: return GL_FRAGMENT_SHADER;
            case gfx::ShaderStage::vertex: return GL_VERTEX_SHADER;
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_shader_stages(gfx::ShaderStage const shader_stage) -> std::int32_t {
        switch (shader_stage) {
            case gfx::ShaderStage::fragment: return GL_FRAGMENT_SHADER_BIT;
            case gfx::ShaderStage::vertex: return GL_VERTEX_SHADER_BIT;
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_compare_func(gfx::CompareOp const compare_op) -> std::int32_t {
        switch (compare_op) {
            case gfx::CompareOp::never: return GL_NEVER;
            case gfx::CompareOp::less: return GL_LESS;
            case gfx::CompareOp::equal: return GL_EQUAL;
            case gfx::CompareOp::less_or_equal: return GL_LEQUAL;
            case gfx::CompareOp::greater: return GL_GREATER;
            case gfx::CompareOp::not_equal: return GL_NOTEQUAL;
            case gfx::CompareOp::greater_or_equal: return GL_GEQUAL;
            case gfx::CompareOp::always: return GL_ALWAYS;
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_buffer_target(gfx::BufferUsage const buffer_usage) -> std::uint32_t {
        switch (buffer_usage) {
            case gfx::BufferUsage::elements: return GL_ELEMENT_ARRAY_BUFFER;
            case gfx::BufferUsage::vertices: return GL_ARRAY_BUFFER;
            case gfx::BufferUsage::uniform: return GL_UNIFORM_BUFFER;
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_blend_factor(gfx::BlendFactor const blend_factor) -> std::uint32_t {
        switch (blend_factor) {
            case gfx::BlendFactor::zero: return GL_ZERO;
            case gfx::BlendFactor::one: return GL_ONE;
            case gfx::BlendFactor::src_color: return GL_SRC_COLOR;
            case gfx::BlendFactor::one_minus_src_color: return GL_ONE_MINUS_SRC_COLOR;
            case gfx::BlendFactor::dst_color: return GL_DST_COLOR;
            case gfx::BlendFactor::one_minus_dst_color: return GL_ONE_MINUS_DST_COLOR;
            case gfx::BlendFactor::src_alpha: return GL_SRC_ALPHA;
            case gfx::BlendFactor::one_minus_src_alpha: return GL_ONE_MINUS_SRC_ALPHA;
            case gfx::BlendFactor::dst_alpha: return GL_DST_ALPHA;
            case gfx::BlendFactor::one_minus_dst_alpha: return GL_ONE_MINUS_DST_ALPHA;
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_blend_op(gfx::BlendOp const blend_op) -> GLenum {
        switch (blend_op) {
            case gfx::BlendOp::add: return GL_FUNC_ADD;
            case gfx::BlendOp::subtract: return GL_FUNC_SUBTRACT;
            case gfx::BlendOp::reverse_subtract: return GL_FUNC_REVERSE_SUBTRACT;
            case gfx::BlendOp::min: return GL_MIN;
            case gfx::BlendOp::max: return GL_MAX;
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_min_filter(gfx::Filter const filter, gfx::SamplerMipmapMode const sampler_mipmap_mode) -> GLint {
        switch (filter) {
            case gfx::Filter::nearest: switch (sampler_mipmap_mode) {
                case gfx::SamplerMipmapMode::nearest: return GL_NEAREST_MIPMAP_NEAREST;
                case gfx::SamplerMipmapMode::linear: return GL_NEAREST_MIPMAP_LINEAR;
                default: std::unreachable();
            }
            case gfx::Filter::linear: switch (sampler_mipmap_mode) {
                case gfx::SamplerMipmapMode::nearest: return GL_LINEAR_MIPMAP_NEAREST;
                case gfx::SamplerMipmapMode::linear: return GL_LINEAR_MIPMAP_LINEAR;
                default: std::unreachable();
            }
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_mag_filter(gfx::Filter const filter) -> GLint {
        switch (filter) {
            case gfx::Filter::nearest: return GL_NEAREST;
            case gfx::Filter::linear: return GL_LINEAR;
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_wrap(gfx::SamplerAddressMode const sampler_address_mode) -> GLint {
        switch (sampler_address_mode) {
            case gfx::SamplerAddressMode::repeat: return GL_REPEAT;
            case gfx::SamplerAddressMode::mirror_repeat: return GL_MIRRORED_REPEAT;
            case gfx::SamplerAddressMode::clamp_to_edge: return GL_CLAMP_TO_EDGE;
            case gfx::SamplerAddressMode::clamp_to_border: return GL_CLAMP_TO_BORDER;
            case gfx::SamplerAddressMode::mirror_clamp_to_edge: return GL_MIRROR_CLAMP_TO_EDGE; // GL 4.1 core
            default: std::unreachable();
        }
    }
    constexpr auto __to_gl_stencil_op(gfx::StencilOp const stencil_op) -> GLenum {
        switch (stencil_op) {
            case gfx::StencilOp::keep: return GL_KEEP;
            case gfx::StencilOp::zero: return GL_ZERO;
            case gfx::StencilOp::replace: return GL_REPLACE;
            case gfx::StencilOp::increment_and_clamp: return GL_INCR;
            case gfx::StencilOp::decrement_and_clamp: return GL_DECR;
            case gfx::StencilOp::invert: return GL_INVERT;
            case gfx::StencilOp::increment_and_wrap: return GL_INCR_WRAP;
            case gfx::StencilOp::decrement_and_wrap: return GL_DECR_WRAP;
            default: std::unreachable();
        }
    }

    constexpr auto __to_gl_face(gfx::StencilFace const stencil_face) -> GLenum {
        switch (stencil_face) {
            case gfx::StencilFace::front: return GL_FRONT;
            case gfx::StencilFace::back: return GL_BACK;
            case gfx::StencilFace::front_and_back: return GL_FRONT_AND_BACK;
            default: std::unreachable();
        }
    }

    // constexpr void __to_gl_border_color(gfx::BorderColor bc, GLfloat out_rgba[4]) {
    //     switch (bc) {
    //         case VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK: out_rgba[0]=0; out_rgba[1]=0; out_rgba[2]=0; out_rgba[3]=0; break;
    //         case VK_BORDER_COLOR_INT_TRANSPARENT_BLACK:   out_rgba[0]=0; out_rgba[1]=0; out_rgba[2]=0; out_rgba[3]=0; break;
    //         case VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK:      out_rgba[0]=0; out_rgba[1]=0; out_rgba[2]=0; out_rgba[3]=1; break;
    //         case VK_BORDER_COLOR_INT_OPAQUE_BLACK:        out_rgba[0]=0; out_rgba[1]=0; out_rgba[2]=0; out_rgba[3]=1; break;
    //         case VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE:      out_rgba[0]=1; out_rgba[1]=1; out_rgba[2]=1; out_rgba[3]=1; break;
    //         case VK_BORDER_COLOR_INT_OPAQUE_WHITE:        out_rgba[0]=1; out_rgba[1]=1; out_rgba[2]=1; out_rgba[3]=1; break;
    //         default:                                      out_rgba[0]=0; out_rgba[1]=0; out_rgba[2]=0; out_rgba[3]=0; break;
    //     }
    // }
}

struct StencilOpState {
    gfx::StencilOp failOp;
    gfx::StencilOp passOp;
    gfx::StencilOp depthFailOp;
    gfx::CompareOp compareOp;
    std::uint32_t compareMask;
    std::uint32_t writeMask;
    std::uint32_t reference;
};

struct gfx::Context_T {
    void* image;
};

struct gfx::Device_T {
    void (*__glFlush)(void);

    // 1. Error and state
    GLenum (*__glGetError)(void);
    void (*__glGetFloatv)(GLenum pname, GLfloat* data);
    void (*__glGetIntegerv)(GLenum pname, GLint* data);

    // 2. Vertex Array Objects
    void (*__glGenVertexArrays)(GLsizei n, GLuint* arrays);
    void (*__glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);
    void (*__glBindVertexArray)(GLuint array);

    // 3. Vertex Attributes
    void (*__glEnableVertexAttribArray)(GLuint index);
    void (*__glDisableVertexAttribArray)(GLuint index);
    void (*__glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
    void (*__glVertexAttribDivisor)(GLuint index, GLuint divisor);


    // 4. Buffer Objects
    void (*__glGenBuffers)(GLsizei n, GLuint* buffers);
    void (*__glDeleteBuffers)(GLsizei n, const GLuint* buffers);
    void (*__glBindBuffer)(GLenum target, GLuint buffer);
    void (*__glBindBufferBase)(GLenum target, GLuint index, GLuint buffer);
    void (*__glBindBufferRange)(GLenum target, GLuint index, GLuint buffer, GLintptr offset, GLsizeiptr size);
    void (*__glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
    void (*__glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
    void* (*__glMapBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length, GLbitfield access);
    GLboolean (*__glUnmapBuffer)(GLenum target);
    void (*__glFlushMappedBufferRange)(GLenum target, GLintptr offset, GLsizeiptr length);
    void (*__glCopyBufferSubData)(GLenum readTarget, GLenum writeTarget, GLintptr readOffset, GLintptr writeOffset, GLsizeiptr size);

    // 5. Shader Objects
    GLuint (*__glCreateShader)(GLenum type);
    void (*__glDeleteShader)(GLuint shader);
    void (*__glShaderSource)(GLuint shader, GLsizei count, const GLchar* const*string, const GLint* length);
    void (*__glCompileShader)(GLuint shader);
    void (*__glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
    void (*__glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);

    // 6. Program Objects
    GLuint (*__glCreateProgram)(void);
    void (*__glDeleteProgram)(GLuint program);
    void (*__glLinkProgram)(GLuint program);
    void (*__glAttachShader)(GLuint program, GLuint shader);
    void (*__glDetachShader)(GLuint program, GLuint shader);
    void (*__glUseProgram)(GLuint program);
    void (*__glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
    void (*__glGetProgramInfoLog)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);

    // 7. Framebuffers and Clearing
    void (*__glBindFramebuffer)(GLenum target, GLuint framebuffer);
    void (*__glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat* value);
    void (*__glClearBufferiv)(GLenum buffer, GLint drawbuffer, const GLint* value);

    // 8. Viewport & Scissor
    void (*__glScissorIndexed)(GLuint index, GLint left, GLint bottom, GLsizei width, GLsizei height);
    void (*__glViewportIndexedf)(GLuint index, GLfloat x, GLfloat y, GLfloat w, GLfloat h);

    // 9. Enable/Disable State
    void (*__glEnable)(GLenum cap);
    void (*__glDisable)(GLenum cap);
    void (*__glEnablei)(GLenum cap, GLuint index);
    void (*__glDisablei)(GLenum cap, GLuint index);
    void (*__glCullFace)(GLenum mode);
    void (*__glDepthFunc)(GLenum func);
    void (*__glDepthMask)(GLboolean flag);
    void (*__glBlendFuncSeparatei)(GLuint buf, GLenum srcRGB, GLenum dstRGB, GLenum srcAlpha, GLenum dstAlpha);
    void (*__glBlendEquationSeparatei)(GLuint buf, GLenum modeRGB, GLenum modeAlpha);

    // 10. Drawing
    void (*__glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
    void (*__glDrawElementsInstancedBaseVertex)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount, GLint basevertex);

    // 11. Program Pipelines
    void (*__glGenProgramPipelines)(GLsizei n, GLuint* pipelines);
    void (*__glDeleteProgramPipelines)(GLsizei n, const GLuint* pipelines);
    void (*__glProgramParameteri)(GLuint program, GLenum pname, GLint value);
    void (*__glBindProgramPipeline)(GLuint pipeline);
    void (*__glUseProgramStages)(GLuint pipeline, GLbitfield stages, GLuint program);
    void (*__glProgramUniformMatrix4fv)(GLuint program, GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

    GLuint (*__glGetUniformBlockIndex)(GLuint program, const GLchar*  uniformBlockName);

    void (*__glUniformBlockBinding)(GLuint program, GLuint uniformBlockIndex, GLuint uniformBlockBinding);
    void (*__glBindAttribLocation)(GLuint program, GLuint index, const GLchar* name);

    GLint (*__glGetUniformLocation)(GLuint program, const GLchar* name);
    void (*__glProgramUniform1i)(GLuint program, GLint location, GLint v0);

    // 11. Fence
    GLsync (*__glFenceSync)(GLenum condition, GLbitfield flags);
    void (*__glDeleteSync)(GLsync sync);
    GLenum (*__glClientWaitSync)(GLsync sync, GLbitfield flags, GLuint64 timeout);

    // 12. Texture & Sampler
    void(*__glGenTextures)(GLsizei n, GLuint* textures);
    void(*__glDeleteTextures)(GLsizei n, const GLuint* textures);
    void(*__glTexParameteri)(GLenum target, GLenum pname, GLint param);
    void(*__glBindTexture)(GLenum target, GLuint texture);
    void(*__glActiveTexture)(GLenum texture);
    void(*__glTexImage1D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
    void(*__glTexImage2D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
    void(*__glTexImage3D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid* pixels);
    void(*__glTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const void* pixels);
    void(*__glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
    void(*__glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
    void(*__glPixelStorei)(GLenum pname, GLint param);
    void(*__glGenSamplers)(GLsizei n, GLuint* samplers);
    void(*__glDeleteSamplers)(GLsizei n, const GLuint* samplers);
    void(*__glSamplerParameteri)(GLuint, GLenum, GLint);
    void(*__glSamplerParameterf)(GLuint, GLenum, GLfloat);
    void(*__glSamplerParameterfv)(GLuint, GLenum, const GLfloat*);
    void(*__glBindSampler)(GLuint unit, GLuint sampler);

    // Stencil & color mask
    void (*__glStencilMask)(GLuint mask);
    void (*__glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
    void (*__glStencilMaskSeparate)(GLenum face, GLuint mask);
    void (*__glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);

    void (*__glStencilFunc)(GLenum func, GLint ref, GLuint mask);
    void (*__glStencilOp)(GLenum sfail, GLenum dpfail, GLenum dppass);
    void (*__glColorMask)(GLboolean r, GLboolean g, GLboolean b, GLboolean a);

    std::int32_t uniform_buffer_offset_alignment;
};

struct gfx::Fence_T {
    GLsync gl_sync;
};

struct gfx::Image_T {
    GLuint gl_texture;
    GLenum gl_target;
    ImageType type;
    Format format;
    std::uint32_t width;
    std::uint32_t height;
    std::uint32_t depth;
    std::uint32_t mip_levels;
    std::uint32_t array_layers;
};

struct gfx::Sampler_T {
    GLuint gl_sampler;
};

struct gfx::Buffer_T {
    std::uint32_t gl_buffer;
    std::uint32_t gl_target;
    std::uint32_t gl_usage;
    std::size_t size;
};

struct gfx::ShaderModule_T {
    std::unique_ptr<spirv_cross::CompilerGLSL> compiler;
};

struct gfx::ShaderObject_T {
    std::uint32_t gl_program;
};

struct gfx::RenderPipeline_T {
    ShaderObject vertex_shader = {};
    ShaderObject fragment_shader = {};
    CullMode cull_mode = {};
    CompareOp depth_compare_op = {};
    bool is_depth_write_enabled = {};
    std::vector<VertexBinding> vertex_bindings = {};
    std::vector<VertexAttribute> vertex_attributes = {};
    std::vector<BlendAttachmentState> blend_attachments = {};
};

enum CmdType {
    CMD_TYPE_BEGIN_RENDER_PASS,
    CMD_TYPE_SET_SCISSOR,
    CMD_TYPE_SET_VIEWPORT,
    CMD_TYPE_SET_RENDER_PIPELINE,
    CMD_TYPE_SET_VERTEX_BUFFERS,
    CMD_TYPE_BIND_BUFFER,
    CMD_TYPE_BIND_COMBINED_IMAGE_SAMPLER,
    CMD_TYPE_SET_BYTES,
    CMD_TYPE_DRAW,
    CMD_TYPE_DRAW_INDEXED,

    CMD_TYPE_SET_STENCIL_TEST_ENABLE,
    CMD_TYPE_SET_STENCIL_WRITE_MASK,
    CMD_TYPE_SET_STENCIL_COMPARE_MASK,
    CMD_TYPE_SET_STENCIL_REFERENCE,
    CMD_TYPE_SET_STENCIL_OP,

    CMD_TYPE_SET_COLOR_WRITE_MASK,
    CMD_TYPE_CLEAR_STENCIL,
};

struct CmdBeginRenderPass {
    std::uint32_t gl_framebuffer;
    std::vector<gfx::RenderPassColorAttachmentInfo> color_attachments;
    bool has_depth_attachment;
    std::float_t depth_clear_value;
    bool has_stencil_attachment;
    std::int32_t stencil_clear_value;
};

struct CmdSetScissor {
    std::uint32_t first_index;
    gfx::Rect2D scissor;
};
struct CmdSetViewport {
    std::uint32_t first_index;
    gfx::Viewport viewport;
};
struct CmdSetRenderPipeline {
    gfx::RenderPipeline pipeline;
};
struct CmdSetVertexBuffers {
    std::vector<gfx::Buffer> buffers;
};
struct CmdBindBuffer {
    std::uint32_t binding;
    gfx::Buffer buffer;
};
struct CmdBindCombinedImageSampler {
    std::uint32_t binding;
    gfx::Image image;
    gfx::Sampler sampler;
};
struct CmdSetBytes {
    std::uint32_t binding;
    std::size_t offset;
    std::vector<std::byte> bytes;
};
struct CmdDraw {
    gfx::Topology topology;
    std::uint32_t vertex_count;
    std::uint32_t first_vertex;
    std::uint32_t instance_count;
};
struct CmdDrawIndexed {
    gfx::Topology topology;
    gfx::Buffer index_buffer;
    std::uint32_t index_offset;
    std::uint32_t index_count;
    std::uint32_t base_vertex;
    gfx::IndexType index_type;
    std::uint32_t instance_count;
};
struct CmdSetStencilTestEnable {
    bool stencil_test_enable;
};
struct CmdSetStencilWriteMask {
    gfx::StencilFace face_mask;
    std::uint32_t write_mask;
};
struct CmdSetStencilCompareMask {
    gfx::StencilFace face_mask;
    std::uint32_t compare_mask;
};
struct CmdSetStencilReference {
    gfx::StencilFace face_mask;
    std::uint32_t reference;
};
struct CmdSetStencilOp {
    gfx::StencilFace face_mask;
    gfx::StencilOp fail_op;
    gfx::StencilOp pass_op;
    gfx::StencilOp depth_fail_op;
    gfx::CompareOp compare_op;
};
struct CmdSetColorWriteMask {
    gfx::ColorWriteMask color_write_mask;
};
struct CmdClearStencil {
    std::int32_t clear_value;
};

struct CommandBufferCmd {
    CmdType Type;
    stl::Enum<
    CmdBeginRenderPass,
    CmdSetScissor,
    CmdSetViewport,
    CmdSetRenderPipeline,
    CmdSetVertexBuffers,
    CmdBindBuffer,
    CmdBindCombinedImageSampler,
    CmdSetBytes,
    CmdDraw,
    CmdDrawIndexed,
    CmdSetStencilTestEnable,
    CmdSetStencilWriteMask,
    CmdSetStencilCompareMask,
    CmdSetStencilReference,
    CmdSetStencilOp,
    CmdSetColorWriteMask,
    CmdClearStencil
    > cmd;
};

struct gfx::CommandBuffer_T {
    std::int32_t uniform_buffer_offset_alignment = {};

    std::uint32_t gl_pipeline = {};
    std::uint32_t gl_vertex_array = {};
    std::size_t push_constant_length = {};

    bool stencil_test_enable = {};
    StencilOpState back_stencil_state = {};
    StencilOpState front_stencil_state = {};
    ColorWriteMask color_write_mask = {};

    std::vector<CommandBufferCmd> commands = {};
};

namespace {
    constexpr auto __align_up(std::size_t value, std::size_t alignment) -> std::size_t {
        return (value + alignment - 1) & ~(alignment - 1);
    }

    void __check(std::uint32_t error, std::source_location location = std::source_location::current()) {
        if (error != 0) {
            std::println(stderr, "{}:{}:{}: {}", location.file_name(), location.line(), location.column(), error);
        }
    }

    void __bind_stencil_state(gfx::Device device, gfx::CommandBuffer command_buffer) {
        device->__glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
        __check(device->__glGetError());

        if (command_buffer->stencil_test_enable) {
            device->__glEnable(GL_STENCIL_TEST);
            __check(device->__glGetError());

            device->__glStencilOpSeparate(
                GL_FRONT,
                __to_gl_stencil_op(command_buffer->front_stencil_state.failOp),
                __to_gl_stencil_op(command_buffer->front_stencil_state.depthFailOp),
                __to_gl_stencil_op(command_buffer->front_stencil_state.passOp)
            );
            __check(device->__glGetError());
            device->__glStencilMaskSeparate(GL_FRONT, command_buffer->front_stencil_state.writeMask);
            __check(device->__glGetError());

            device->__glStencilFuncSeparate(
                GL_FRONT,
                __to_gl_compare_func(command_buffer->front_stencil_state.compareOp),
                static_cast<GLint>(command_buffer->front_stencil_state.reference),
                command_buffer->front_stencil_state.compareMask
            );
            __check(device->__glGetError());

            device->__glStencilOpSeparate(
                GL_BACK,
                __to_gl_stencil_op(command_buffer->back_stencil_state.failOp),
                __to_gl_stencil_op(command_buffer->back_stencil_state.depthFailOp),
                __to_gl_stencil_op(command_buffer->back_stencil_state.passOp)
            );
            __check(device->__glGetError());

            device->__glStencilMaskSeparate(GL_BACK, command_buffer->back_stencil_state.writeMask);
            __check(device->__glGetError());

            device->__glStencilFuncSeparate(
                GL_BACK,
                __to_gl_compare_func(command_buffer->back_stencil_state.compareOp),
                static_cast<GLint>(command_buffer->back_stencil_state.reference),
                command_buffer->back_stencil_state.compareMask
            );
            __check(device->__glGetError());

            device->__glColorMask(
                command_buffer->color_write_mask.r,
                command_buffer->color_write_mask.g,
                command_buffer->color_write_mask.b,
                command_buffer->color_write_mask.a
            );
        } else {
            device->__glDisable(GL_STENCIL_TEST);
            __check(device->__glGetError());
        }
    }

    void __bind_attributes(gfx::Device device, gfx::CommandBuffer command_buffer, std::span<gfx::VertexBinding const> bindings, std::span<gfx::VertexAttribute const> attributes, std::span<gfx::Buffer const> buffers) {
        device->__glBindVertexArray(command_buffer->gl_vertex_array);
        __check(device->__glGetError());
        for (std::size_t i = 0; i < attributes.size(); i += 1) {
            auto const [size, type, normalized] = [&] {
                switch (attributes[i].format) {
                    case gfx::Format::r8g8_unorm: return std::tuple(2, GL_UNSIGNED_BYTE, GL_TRUE);
                    case gfx::Format::r8g8b8_unorm: return std::tuple(3, GL_UNSIGNED_BYTE, GL_TRUE);
                    case gfx::Format::r8g8b8a8_unorm: return std::tuple(4, GL_UNSIGNED_BYTE, GL_TRUE);
                    case gfx::Format::r32g32_sfloat: return std::tuple(2, GL_FLOAT, GL_FALSE);
                    case gfx::Format::r32g32b32_sfloat: return std::tuple(3, GL_FLOAT, GL_FALSE);
                    case gfx::Format::r32g32b32a32_sfloat: return std::tuple(4, GL_FLOAT, GL_FALSE);
                    default: std::unreachable();
                }
            }();

            device->__glBindBuffer(GL_ARRAY_BUFFER, buffers[attributes[i].binding]->gl_buffer);
            __check(device->__glGetError());
            device->__glVertexAttribPointer(attributes[i].location, size, type, normalized, static_cast<GLsizei>(bindings[attributes[i].binding].stride), reinterpret_cast<void*>(static_cast<std::uintptr_t>(attributes[i].offset)));
            __check(device->__glGetError());
            device->__glEnableVertexAttribArray(attributes[i].location);
            __check(device->__glGetError());
            switch (bindings[attributes[i].binding].input_rate) {
                case gfx::VertexInputRate::vertex:
                    device->__glVertexAttribDivisor(attributes[i].location, 0);
                    __check(device->__glGetError());
                    break;
                case gfx::VertexInputRate::instance:
                    device->__glVertexAttribDivisor(attributes[i].location, 1);
                    __check(device->__glGetError());
                    break;
            }
        }
    }

    void __unbind_attributes(gfx::Device device, gfx::CommandBuffer command_buffer, std::span<gfx::VertexBinding const> bindings, std::span<gfx::VertexAttribute const> attributes, std::span<gfx::Buffer const> buffers) {
        for (std::size_t i = 0; i < attributes.size(); i += 1) {
            device->__glDisableVertexAttribArray(attributes[i].location);
            __check(device->__glGetError());
        }
        device->__glBindVertexArray(GL_NONE);
        __check(device->__glGetError());
    }
}

auto gfx::CreateContext() -> Context {
    auto* context = new Context_T();
    context->image = ::dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);
    return context;
}

void gfx::DeleteContext(Context context) {
    dlclose(context->image);
}

auto gfx::GetProcAddress(Context context, char const* name) -> void* {
    return dlsym(context->image, name);
}

auto gfx::CreateDevice(Context context) -> Device {
    auto* device = new Device_T();
    device->__glFlush = reinterpret_cast<decltype(Device_T::__glFlush)>(GetProcAddress(context, "glFlush"));
    device->__glGetError = reinterpret_cast<decltype(Device_T::__glGetError)>(GetProcAddress(context, "glGetError"));
    device->__glGetFloatv = reinterpret_cast<decltype(Device_T::__glGetFloatv)>(GetProcAddress(context, "glGetFloatv"));
    device->__glGetIntegerv = reinterpret_cast<decltype(Device_T::__glGetIntegerv)>(GetProcAddress(context, "glGetIntegerv"));
    device->__glGenVertexArrays = reinterpret_cast<decltype(Device_T::__glGenVertexArrays)>(GetProcAddress(context, "glGenVertexArrays"));
    device->__glDeleteVertexArrays = reinterpret_cast<decltype(Device_T::__glDeleteVertexArrays)>(GetProcAddress(context, "glDeleteVertexArrays"));
    device->__glBindVertexArray = reinterpret_cast<decltype(Device_T::__glBindVertexArray)>(GetProcAddress(context, "glBindVertexArray"));
    device->__glEnableVertexAttribArray = reinterpret_cast<decltype(Device_T::__glEnableVertexAttribArray)>(GetProcAddress(context, "glEnableVertexAttribArray"));
    device->__glDisableVertexAttribArray = reinterpret_cast<decltype(Device_T::__glDisableVertexAttribArray)>(GetProcAddress(context, "glDisableVertexAttribArray"));
    device->__glVertexAttribPointer = reinterpret_cast<decltype(Device_T::__glVertexAttribPointer)>(GetProcAddress(context, "glVertexAttribPointer"));
    device->__glVertexAttribDivisor = reinterpret_cast<decltype(Device_T::__glVertexAttribDivisor)>(GetProcAddress(context, "glVertexAttribDivisor"));
    device->__glGenBuffers = reinterpret_cast<decltype(Device_T::__glGenBuffers)>(GetProcAddress(context, "glGenBuffers"));
    device->__glDeleteBuffers = reinterpret_cast<decltype(Device_T::__glDeleteBuffers)>(GetProcAddress(context, "glDeleteBuffers"));
    device->__glBindBuffer = reinterpret_cast<decltype(Device_T::__glBindBuffer)>(GetProcAddress(context, "glBindBuffer"));
    device->__glBindBufferBase = reinterpret_cast<decltype(Device_T::__glBindBufferBase)>(GetProcAddress(context, "glBindBufferBase"));
    device->__glBindBufferRange = reinterpret_cast<decltype(Device_T::__glBindBufferRange)>(GetProcAddress(context, "glBindBufferRange"));
    device->__glBufferData = reinterpret_cast<decltype(Device_T::__glBufferData)>(GetProcAddress(context, "glBufferData"));
    device->__glBufferSubData = reinterpret_cast<decltype(Device_T::__glBufferSubData)>(GetProcAddress(context, "glBufferSubData"));
    device->__glMapBufferRange = reinterpret_cast<decltype(Device_T::__glMapBufferRange)>(GetProcAddress(context, "glMapBufferRange"));
    device->__glUnmapBuffer = reinterpret_cast<decltype(Device_T::__glUnmapBuffer)>(GetProcAddress(context, "glUnmapBuffer"));
    device->__glFlushMappedBufferRange = reinterpret_cast<decltype(Device_T::__glFlushMappedBufferRange)>(GetProcAddress(context, "glFlushMappedBufferRange"));
    device->__glCopyBufferSubData = reinterpret_cast<decltype(Device_T::__glCopyBufferSubData)>(GetProcAddress(context, "glCopyBufferSubData"));
    device->__glCreateShader = reinterpret_cast<decltype(Device_T::__glCreateShader)>(GetProcAddress(context, "glCreateShader"));
    device->__glDeleteShader = reinterpret_cast<decltype(Device_T::__glDeleteShader)>(GetProcAddress(context, "glDeleteShader"));
    device->__glShaderSource = reinterpret_cast<decltype(Device_T::__glShaderSource)>(GetProcAddress(context, "glShaderSource"));
    device->__glCompileShader = reinterpret_cast<decltype(Device_T::__glCompileShader)>(GetProcAddress(context, "glCompileShader"));
    device->__glGetShaderiv = reinterpret_cast<decltype(Device_T::__glGetShaderiv)>(GetProcAddress(context, "glGetShaderiv"));
    device->__glGetShaderInfoLog = reinterpret_cast<decltype(Device_T::__glGetShaderInfoLog)>(GetProcAddress(context, "glGetShaderInfoLog"));
    device->__glCreateProgram = reinterpret_cast<decltype(Device_T::__glCreateProgram)>(GetProcAddress(context, "glCreateProgram"));
    device->__glDeleteProgram = reinterpret_cast<decltype(Device_T::__glDeleteProgram)>(GetProcAddress(context, "glDeleteProgram"));
    device->__glLinkProgram = reinterpret_cast<decltype(Device_T::__glLinkProgram)>(GetProcAddress(context, "glLinkProgram"));
    device->__glAttachShader = reinterpret_cast<decltype(Device_T::__glAttachShader)>(GetProcAddress(context, "glAttachShader"));
    device->__glDetachShader = reinterpret_cast<decltype(Device_T::__glDetachShader)>(GetProcAddress(context, "glDetachShader"));
    device->__glUseProgram = reinterpret_cast<decltype(Device_T::__glUseProgram)>(GetProcAddress(context, "glUseProgram"));
    device->__glGetProgramiv = reinterpret_cast<decltype(Device_T::__glGetProgramiv)>(GetProcAddress(context, "glGetProgramiv"));
    device->__glGetProgramInfoLog = reinterpret_cast<decltype(Device_T::__glGetProgramInfoLog)>(GetProcAddress(context, "glGetProgramInfoLog"));
    device->__glBindFramebuffer = reinterpret_cast<decltype(Device_T::__glBindFramebuffer)>(GetProcAddress(context, "glBindFramebuffer"));
    device->__glClearBufferfv = reinterpret_cast<decltype(Device_T::__glClearBufferfv)>(GetProcAddress(context, "glClearBufferfv"));
    device->__glClearBufferiv = reinterpret_cast<decltype(Device_T::__glClearBufferiv)>(GetProcAddress(context, "glClearBufferiv"));
    device->__glScissorIndexed = reinterpret_cast<decltype(Device_T::__glScissorIndexed)>(GetProcAddress(context, "glScissorIndexed"));
    device->__glViewportIndexedf = reinterpret_cast<decltype(Device_T::__glViewportIndexedf)>(GetProcAddress(context, "glViewportIndexedf"));
    device->__glEnable = reinterpret_cast<decltype(Device_T::__glEnable)>(GetProcAddress(context, "glEnable"));
    device->__glDisable = reinterpret_cast<decltype(Device_T::__glDisable)>(GetProcAddress(context, "glDisable"));
    device->__glEnablei = reinterpret_cast<decltype(Device_T::__glEnablei)>(GetProcAddress(context, "glEnablei"));
    device->__glDisablei = reinterpret_cast<decltype(Device_T::__glDisablei)>(GetProcAddress(context, "glDisablei"));
    device->__glCullFace = reinterpret_cast<decltype(Device_T::__glCullFace)>(GetProcAddress(context, "glCullFace"));
    device->__glDepthFunc = reinterpret_cast<decltype(Device_T::__glDepthFunc)>(GetProcAddress(context, "glDepthFunc"));
    device->__glDepthMask = reinterpret_cast<decltype(Device_T::__glDepthMask)>(GetProcAddress(context, "glDepthMask"));
    device->__glBlendFuncSeparatei = reinterpret_cast<decltype(Device_T::__glBlendFuncSeparatei)>(GetProcAddress(context, "glBlendFuncSeparatei"));
    device->__glBlendEquationSeparatei = reinterpret_cast<decltype(Device_T::__glBlendEquationSeparatei)>(GetProcAddress(context, "glBlendEquationSeparatei"));
    device->__glDrawArraysInstanced = reinterpret_cast<decltype(Device_T::__glDrawArraysInstanced)>(GetProcAddress(context, "glDrawArraysInstanced"));
    device->__glDrawElementsInstancedBaseVertex = reinterpret_cast<decltype(Device_T::__glDrawElementsInstancedBaseVertex)>(GetProcAddress(context, "glDrawElementsInstancedBaseVertex"));
    device->__glGenProgramPipelines = reinterpret_cast<decltype(Device_T::__glGenProgramPipelines)>(GetProcAddress(context, "glGenProgramPipelines"));
    device->__glDeleteProgramPipelines = reinterpret_cast<decltype(Device_T::__glDeleteProgramPipelines)>(GetProcAddress(context, "glDeleteProgramPipelines"));
    device->__glProgramParameteri = reinterpret_cast<decltype(Device_T::__glProgramParameteri)>(GetProcAddress(context, "glProgramParameteri"));
    device->__glBindProgramPipeline = reinterpret_cast<decltype(Device_T::__glBindProgramPipeline)>(GetProcAddress(context, "glBindProgramPipeline"));
    device->__glUseProgramStages = reinterpret_cast<decltype(Device_T::__glUseProgramStages)>(GetProcAddress(context, "glUseProgramStages"));
    device->__glProgramUniformMatrix4fv = reinterpret_cast<decltype(Device_T::__glProgramUniformMatrix4fv)>(GetProcAddress(context, "glProgramUniformMatrix4fv"));
    device->__glGetUniformBlockIndex = reinterpret_cast<decltype(Device_T::__glGetUniformBlockIndex)>(GetProcAddress(context, "glGetUniformBlockIndex"));
    device->__glUniformBlockBinding = reinterpret_cast<decltype(Device_T::__glUniformBlockBinding)>(GetProcAddress(context, "glUniformBlockBinding"));
    device->__glBindAttribLocation = reinterpret_cast<decltype(Device_T::__glBindAttribLocation)>(GetProcAddress(context, "glBindAttribLocation"));
    device->__glGetUniformLocation = reinterpret_cast<decltype(Device_T::__glGetUniformLocation)>(GetProcAddress(context, "glGetUniformLocation"));
    device->__glProgramUniform1i = reinterpret_cast<decltype(Device_T::__glProgramUniform1i)>(GetProcAddress(context, "glProgramUniform1i"));
    device->__glFenceSync = reinterpret_cast<decltype(Device_T::__glFenceSync)>(GetProcAddress(context, "glFenceSync"));
    device->__glDeleteSync = reinterpret_cast<decltype(Device_T::__glDeleteSync)>(GetProcAddress(context, "glDeleteSync"));
    device->__glClientWaitSync = reinterpret_cast<decltype(Device_T::__glClientWaitSync)>(GetProcAddress(context, "glClientWaitSync"));
    device->__glGenTextures = reinterpret_cast<decltype(Device_T::__glGenTextures)>(GetProcAddress(context, "glGenTextures"));
    device->__glDeleteTextures = reinterpret_cast<decltype(Device_T::__glDeleteTextures)>(GetProcAddress(context, "glDeleteTextures"));
    device->__glTexParameteri = reinterpret_cast<decltype(Device_T::__glTexParameteri)>(GetProcAddress(context, "glTexParameteri"));
    device->__glBindTexture = reinterpret_cast<decltype(Device_T::__glBindTexture)>(GetProcAddress(context, "glBindTexture"));
    device->__glActiveTexture = reinterpret_cast<decltype(Device_T::__glActiveTexture)>(GetProcAddress(context, "glActiveTexture"));
    device->__glTexImage1D = reinterpret_cast<decltype(Device_T::__glTexImage1D)>(GetProcAddress(context, "glTexImage1D"));
    device->__glTexImage2D = reinterpret_cast<decltype(Device_T::__glTexImage2D)>(GetProcAddress(context, "glTexImage2D"));
    device->__glTexImage3D = reinterpret_cast<decltype(Device_T::__glTexImage3D)>(GetProcAddress(context, "glTexImage3D"));
    device->__glTexSubImage1D = reinterpret_cast<decltype(Device_T::__glTexSubImage1D)>(GetProcAddress(context, "glTexSubImage1D"));
    device->__glTexSubImage2D = reinterpret_cast<decltype(Device_T::__glTexSubImage2D)>(GetProcAddress(context, "glTexSubImage2D"));
    device->__glTexSubImage3D = reinterpret_cast<decltype(Device_T::__glTexSubImage3D)>(GetProcAddress(context, "glTexSubImage3D"));
    device->__glPixelStorei = reinterpret_cast<decltype(Device_T::__glPixelStorei)>(GetProcAddress(context, "glPixelStorei"));
    device->__glGenSamplers = reinterpret_cast<decltype(Device_T::__glGenSamplers)>(GetProcAddress(context, "glGenSamplers"));
    device->__glDeleteSamplers = reinterpret_cast<decltype(Device_T::__glDeleteSamplers)>(GetProcAddress(context, "glDeleteSamplers"));
    device->__glSamplerParameteri = reinterpret_cast<decltype(Device_T::__glSamplerParameteri)>(GetProcAddress(context, "glSamplerParameteri"));
    device->__glSamplerParameterf = reinterpret_cast<decltype(Device_T::__glSamplerParameterf)>(GetProcAddress(context, "glSamplerParameterf"));
    device->__glSamplerParameterfv = reinterpret_cast<decltype(Device_T::__glSamplerParameterfv)>(GetProcAddress(context, "glSamplerParameterfv"));
    device->__glBindSampler = reinterpret_cast<decltype(Device_T::__glBindSampler)>(GetProcAddress(context, "glBindSampler"));
    device->__glStencilMask = reinterpret_cast<decltype(Device_T::__glStencilMask)>(GetProcAddress(context, "glStencilMask"));
    device->__glStencilOpSeparate = reinterpret_cast<decltype(Device_T::__glStencilOpSeparate)>(GetProcAddress(context, "glStencilOpSeparate"));
    device->__glStencilMaskSeparate = reinterpret_cast<decltype(Device_T::__glStencilMaskSeparate)>(GetProcAddress(context, "glStencilMaskSeparate"));
    device->__glStencilFuncSeparate = reinterpret_cast<decltype(Device_T::__glStencilFuncSeparate)>(GetProcAddress(context, "glStencilFuncSeparate"));
    device->__glStencilFunc = reinterpret_cast<decltype(Device_T::__glStencilFunc)>(GetProcAddress(context, "glStencilFunc"));
    device->__glStencilOp   = reinterpret_cast<decltype(Device_T::__glStencilOp)>(GetProcAddress(context, "glStencilOp"));
    device->__glColorMask   = reinterpret_cast<decltype(Device_T::__glColorMask)>(GetProcAddress(context, "glColorMask"));
    device->__glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &device->uniform_buffer_offset_alignment);
    return device;
}

void gfx::DeleteDevice(Context context, Device device) {
    delete device;
}

auto gfx::CreateBuffer(Device device, BufferCreateInfo const& create_info) -> Buffer {
    auto const gl_target = __to_gl_buffer_target(create_info.usage);
    auto const align_size = __align_up(create_info.size, device->uniform_buffer_offset_alignment);
    std::uint32_t gl_buffer;
    device->__glGenBuffers(1, &gl_buffer);
    __check(device->__glGetError());
    device->__glBindBuffer(gl_target, gl_buffer);
    __check(device->__glGetError());
    device->__glBufferData(gl_target, static_cast<i64>(align_size), nullptr, create_info.mode);
    __check(device->__glGetError());
    device->__glBindBuffer(gl_target, GL_NONE);
    __check(device->__glGetError());
    return new Buffer_T(gl_buffer, gl_target, create_info.mode, align_size);
}

void gfx::DeleteBuffer(Device device, Buffer buffer) {
    device->__glDeleteBuffers(1, &buffer->gl_buffer);
    __check(device->__glGetError());
    delete buffer;
}

void gfx::UpdateBuffer(Device device, Buffer buffer, std::size_t offset, std::size_t size, void const* data) {
    device->__glBindBuffer(buffer->gl_target, buffer->gl_buffer);
    __check(device->__glGetError());
    device->__glBufferSubData(buffer->gl_target, static_cast<std::int32_t>(offset), static_cast<i64>(size), data);
    __check(device->__glGetError());
    device->__glBindBuffer(buffer->gl_target, GL_NONE);
    __check(device->__glGetError());
}

auto gfx::CreateShaderModule(Device device, ShaderCreateInfo const& create_info) -> ShaderModule {
    std::vector<std::uint32_t> spirv(create_info.bytes.size() / sizeof(std::uint32_t));
    std::memcpy(spirv.data(), create_info.bytes.data(), create_info.bytes.size());

    auto compiler = std::make_unique<spirv_cross::CompilerGLSL>(std::move(spirv));
    auto options = compiler->get_common_options();
    options.version = 410;
    options.es = false;
    options.vulkan_semantics = false;
    options.separate_shader_objects = true;
    options.enable_420pack_extension = false;
    options.emit_push_constant_as_uniform_buffer = true;
    compiler->set_common_options(options);
    return new ShaderModule_T(std::move(compiler));
}

void gfx::DeleteShaderModule(Device device, ShaderModule shader_module) {
    delete shader_module;
}

auto gfx::CreateShaderObject(Device device, ShaderObjectCreateInfo const& create_info) -> ShaderObject {
    switch (create_info.stage) {
        case ShaderStage::fragment:
            create_info.module->compiler->set_entry_point(create_info.entry_point, spv::ExecutionModel::ExecutionModelFragment);
            break;
        case ShaderStage::vertex:
            create_info.module->compiler->set_entry_point(create_info.entry_point, spv::ExecutionModel::ExecutionModelVertex);
            break;
        default:
            std::unreachable();
    }

    auto resources = create_info.module->compiler->get_shader_resources();
    for (auto const& resource : resources.sampled_images) {
        create_info.module->compiler->unset_decoration(resource.id, spv::DecorationDescriptorSet);
    }
    for (auto const& resource : resources.separate_images) {
        create_info.module->compiler->unset_decoration(resource.id, spv::DecorationDescriptorSet);
    }
    for (auto const& resource : resources.separate_samplers) {
        create_info.module->compiler->unset_decoration(resource.id, spv::DecorationDescriptorSet);
    }
    for (auto const& resource : resources.uniform_buffers) {
        create_info.module->compiler->unset_decoration(resource.id, spv::DecorationDescriptorSet);
    }
    for (auto const& resource : resources.gl_plain_uniforms) {
        create_info.module->compiler->unset_decoration(resource.id, spv::DecorationDescriptorSet);
    }

    auto const source = create_info.module->compiler->compile();

    auto const gl_data = source.data();
    auto const gl_size = static_cast<std::int32_t>(source.size());

    auto const gl_stage = __to_gl_shader_type(create_info.stage);
    auto const gl_shader = device->__glCreateShader(gl_stage);
    __check(device->__glGetError());

    device->__glShaderSource(gl_shader, 1, &gl_data, &gl_size);
    __check(device->__glGetError());

    device->__glCompileShader(gl_shader);
    __check(device->__glGetError());

    GLint logLength;
    device->__glGetShaderiv(gl_shader, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 0) {
        std::string msg;
        msg.resize_and_overwrite(logLength, [&](char* data, std::size_t const size) -> std::size_t {
            GLsizei length;
            device->__glGetShaderInfoLog(gl_shader, size, &length, data);
            return length;
        });
        std::println(stdout, "{}", msg);
    }

    auto const gl_program = device->__glCreateProgram();
    __check(device->__glGetError());

    device->__glProgramParameteri(gl_program, GL_PROGRAM_SEPARABLE, GL_TRUE);
    __check(device->__glGetError());

    device->__glAttachShader(gl_program, gl_shader);
    __check(device->__glGetError());

    device->__glLinkProgram(gl_program);
    __check(device->__glGetError());

    device->__glDetachShader(gl_program, gl_shader);
    __check(device->__glGetError());

    device->__glDeleteShader(gl_shader);
    __check(device->__glGetError());

    for (auto const& resource : resources.uniform_buffers) {
        auto const name = create_info.module->compiler->get_remapped_declared_block_name(resource.id);
        auto const binding = create_info.module->compiler->get_decoration(resource.id, spv::DecorationBinding);
        auto const location = device->__glGetUniformBlockIndex(gl_program, name.c_str());
        __check(device->__glGetError());
        if (location != -1) {
            device->__glUniformBlockBinding(gl_program, location, binding);
            __check(device->__glGetError());
        }
    }
    for (auto const& resource : resources.sampled_images) {
        auto const name = create_info.module->compiler->get_remapped_declared_block_name(resource.id);
        auto const binding = create_info.module->compiler->get_decoration(resource.id, spv::DecorationBinding);
        auto const location = device->__glGetUniformLocation(gl_program, name.c_str());
        __check(device->__glGetError());
        if (location != -1) {
            std::println("{}: location = {} -> binding = {}", name, location, binding);
            device->__glProgramUniform1i(gl_program, location, static_cast<GLint>(binding));
            __check(device->__glGetError());
        }
    }
    return new ShaderObject_T(gl_program);
}

void gfx::DeleteShaderObject(Device device, ShaderObject shader_object) {
    device->__glDeleteProgram(shader_object->gl_program);
    delete shader_object;
}

auto gfx::CreateRenderPipeline(Device device, RenderPipelineCreateInfo const& create_info) -> RenderPipeline {
    auto* pipeline = new RenderPipeline_T();
    pipeline->vertex_shader = create_info.vertex_shader;
    pipeline->fragment_shader = create_info.fragment_shader;
    pipeline->cull_mode = create_info.cull_mode;
    pipeline->depth_compare_op = create_info.depth_state.depth_compare_op;
    pipeline->is_depth_write_enabled = create_info.depth_state.is_depth_write_enabled;
    pipeline->blend_attachments.assign_range(create_info.blend_state.attachments);
    pipeline->vertex_bindings.assign_range(create_info.vertex_input_state.bindings);
    pipeline->vertex_attributes.assign_range(create_info.vertex_input_state.attributes);
    return pipeline;
}

void gfx::DeleteRenderPipeline(Device device, RenderPipeline pipeline) {
    __check(device->__glGetError());
    delete pipeline;
}

auto gfx::CreateImage(Device device, ImageCreateInfo const& create_info) -> Image {
    auto const [internal_format, external_format, external_type] = [&create_info] {
        switch (create_info.format) {
            case Format::r8g8b8a8_unorm: return std::tuple(GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE);
            case Format::r32g32_sfloat: return std::tuple(GL_RG32F, GL_RG, GL_FLOAT);
            case Format::r32g32b32_sfloat: return std::tuple(GL_RGB32F, GL_RGB, GL_FLOAT);
            case Format::r32g32b32a32_sfloat: return std::tuple(GL_RGBA32F, GL_RGBA, GL_FLOAT);
            default: std::unreachable();
        }
    }();

    GLuint gl_texture;
    device->__glGenTextures(1, &gl_texture);
    __check(device->__glGetError());

    switch (create_info.type) {
        case ImageType::_1D:
            if (create_info.array_layers > 1) {
                device->__glBindTexture(GL_TEXTURE_1D_ARRAY, gl_texture);
                __check(device->__glGetError());
                for (std::uint32_t i = 0; i < create_info.mip_levels; i += 1) {
                    GLsizei const width = std::max(static_cast<GLsizei>(create_info.extent.width >> i),  1);
                    device->__glTexImage2D(GL_TEXTURE_1D_ARRAY, static_cast<GLint>(i), internal_format, width, static_cast<GLsizei>(create_info.array_layers), 0, external_format, external_type, nullptr);
                    __check(device->__glGetError());
                }
                device->__glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
                __check(device->__glGetError());
                device->__glTexParameteri(GL_TEXTURE_1D_ARRAY, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(create_info.mip_levels - 1));
                __check(device->__glGetError());
                device->__glBindTexture(GL_TEXTURE_1D_ARRAY, 0);
                __check(device->__glGetError());
                return new Image_T(gl_texture, GL_TEXTURE_1D_ARRAY, create_info.type, create_info.format, create_info.extent.width, 1, 1,  create_info.mip_levels, create_info.array_layers);
            } else {
                device->__glBindTexture(GL_TEXTURE_1D, gl_texture);
                __check(device->__glGetError());
                for (std::uint32_t i = 0; i < create_info.mip_levels; i += 1) {
                    GLsizei const width = std::max(static_cast<GLsizei>(create_info.extent.width >> i),  1);
                    device->__glTexImage1D(GL_TEXTURE_1D, static_cast<GLint>(i), internal_format, width, 0, external_format, external_type, nullptr);
                    __check(device->__glGetError());
                }
                device->__glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_BASE_LEVEL, 0);
                __check(device->__glGetError());
                device->__glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(create_info.mip_levels - 1));
                __check(device->__glGetError());
                device->__glBindTexture(GL_TEXTURE_1D, 0);
                __check(device->__glGetError());
                return new Image_T(gl_texture, GL_TEXTURE_1D, create_info.type, create_info.format, create_info.extent.width, 1, 1,  create_info.mip_levels, create_info.array_layers);
            }
        case ImageType::_2D:
            if (create_info.array_layers > 1) {
                device->__glBindTexture(GL_TEXTURE_2D_ARRAY, gl_texture);
                __check(device->__glGetError());
                for (std::uint32_t i = 0; i < create_info.mip_levels; i += 1) {
                    GLsizei const width = std::max(static_cast<GLsizei>(create_info.extent.width >> i), 1);
                    GLsizei const height = std::max(static_cast<GLsizei>(create_info.extent.height >> i), 1);
                    device->__glTexImage3D(GL_TEXTURE_2D_ARRAY, static_cast<GLint>(i), internal_format, width, height, static_cast<GLsizei>(create_info.array_layers), 0, external_format, external_type, nullptr);
                    __check(device->__glGetError());
                }
                device->__glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
                __check(device->__glGetError());
                device->__glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(create_info.mip_levels - 1));
                __check(device->__glGetError());
                device->__glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
                __check(device->__glGetError());
                return new Image_T(gl_texture, GL_TEXTURE_2D_ARRAY, create_info.type, create_info.format, create_info.extent.width, create_info.extent.height, 1,  create_info.mip_levels, create_info.array_layers);
            } else {
                device->__glBindTexture(GL_TEXTURE_2D, gl_texture);
                __check(device->__glGetError());
                for (std::uint32_t i = 0; i < create_info.mip_levels; i += 1) {
                    GLsizei const width = std::max(static_cast<GLsizei>(create_info.extent.width >> i), 1);
                    GLsizei const height = std::max(static_cast<GLsizei>(create_info.extent.height >> i), 1);
                    device->__glTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(i), internal_format, width, height, 0, external_format, external_type, nullptr);
                    __check(device->__glGetError());
                }
                device->__glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
                __check(device->__glGetError());
                device->__glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(create_info.mip_levels - 1));
                __check(device->__glGetError());
                device->__glBindTexture(GL_TEXTURE_2D, 0);
                __check(device->__glGetError());
                return new Image_T(gl_texture, GL_TEXTURE_2D, create_info.type, create_info.format, create_info.extent.width, create_info.extent.height, 1,  create_info.mip_levels, create_info.array_layers);
            }
        case ImageType::_3D: {
            device->__glBindTexture(GL_TEXTURE_3D, gl_texture);
            __check(device->__glGetError());
            for (std::uint32_t i = 0; i < create_info.mip_levels; i += 1) {
                GLsizei const width = std::max(static_cast<GLsizei>(create_info.extent.width >> i), 1);
                GLsizei const height = std::max(static_cast<GLsizei>(create_info.extent.height >> i), 1);
                GLsizei const depth = std::max(static_cast<GLsizei>(create_info.extent.depth >> i), 1);
                device->__glTexImage3D(GL_TEXTURE_3D, static_cast<GLint>(i), internal_format, width, height, depth, 0, external_format, external_type, nullptr);
                __check(device->__glGetError());
            }
            device->__glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_BASE_LEVEL, 0);
            __check(device->__glGetError());
            device->__glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(create_info.mip_levels - 1));
            __check(device->__glGetError());
            device->__glBindTexture(GL_TEXTURE_3D, 0);
            __check(device->__glGetError());
            return new Image_T(gl_texture, GL_TEXTURE_3D, create_info.type, create_info.format, create_info.extent.width, create_info.extent.height, create_info.extent.depth,  create_info.mip_levels, create_info.array_layers);
        }
        default: std::unreachable();
    }
}

void gfx::DeleteImage(Device device, Image image) {
    device->__glDeleteTextures(1, &image->gl_texture);
    __check(device->__glGetError());

    delete image;
}

void gfx::CopyMemoryToImage(Device device, Image image, std::span<MemoryToImageCopy const> regions) {
    auto const [gl_format, gl_type] = [image] {
        switch (image->format) {
            case Format::r8g8b8a8_unorm: return std::tuple(GL_RGBA, GL_UNSIGNED_BYTE);
            case Format::r32g32_sfloat: return std::tuple(GL_RG, GL_FLOAT);
            case Format::r32g32b32_sfloat: return std::tuple(GL_RGB, GL_FLOAT);
            case Format::r32g32b32a32_sfloat: return std::tuple(GL_RGBA, GL_FLOAT);
            default: std::unreachable();
        }
    }();
    device->__glBindTexture(image->gl_target, image->gl_texture);
    __check(device->__glGetError());

    device->__glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    __check(device->__glGetError());

    for (std::size_t i = 0; i < regions.size(); i += 1) {
        device->__glPixelStorei(GL_UNPACK_ROW_LENGTH, static_cast<GLint>(regions[i].memory_row_length));
        __check(device->__glGetError());

        device->__glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, static_cast<GLint>(regions[i].memory_image_height));
        __check(device->__glGetError());

        switch (image->gl_target) {
            case GL_TEXTURE_1D: {
                device->__glTexSubImage1D(GL_TEXTURE_1D, static_cast<GLint>(regions[i].mip_level), regions[i].image_offset.x, static_cast<GLsizei>(regions[i].image_extent.width), gl_format, gl_type, regions[i].host_pointer);
                __check(device->__glGetError());
                break;
            }
            case GL_TEXTURE_1D_ARRAY: {
                device->__glTexSubImage2D(GL_TEXTURE_1D_ARRAY, static_cast<GLint>(regions[i].mip_level), regions[i].image_offset.x, static_cast<GLint>(regions[i].base_array_layer), static_cast<GLsizei>(regions[i].image_extent.width), static_cast<GLsizei>(regions[i].layer_count), gl_format, gl_type, regions[i].host_pointer);
                __check(device->__glGetError());
                break;
            }
            case GL_TEXTURE_2D: {
                device->__glTexSubImage2D(GL_TEXTURE_2D, static_cast<GLint>(regions[i].mip_level), regions[i].image_offset.x, regions[i].image_offset.y, static_cast<GLsizei>(regions[i].image_extent.width), static_cast<GLsizei>(regions[i].image_extent.height), gl_format, gl_type, regions[i].host_pointer);
                __check(device->__glGetError());
                break;
            }
            case GL_TEXTURE_2D_ARRAY: {
                device->__glTexSubImage3D(GL_TEXTURE_2D_ARRAY, static_cast<GLint>(regions[i].mip_level), regions[i].image_offset.x, regions[i].image_offset.y, static_cast<GLint>(regions[i].base_array_layer), static_cast<GLsizei>(regions[i].image_extent.width), static_cast<GLsizei>(regions[i].image_extent.height), static_cast<GLsizei>(regions[i].layer_count), gl_format, gl_type, regions[i].host_pointer);
                __check(device->__glGetError());
                break;
            }
            case GL_TEXTURE_3D: {
                device->__glTexSubImage3D(GL_TEXTURE_3D, static_cast<GLint>(regions[i].mip_level), regions[i].image_offset.x, regions[i].image_offset.y, regions[i].image_offset.z, static_cast<GLsizei>(regions[i].image_extent.width), static_cast<GLsizei>(regions[i].image_extent.height), static_cast<GLsizei>(regions[i].image_extent.depth), gl_format, gl_type, regions[i].host_pointer);
                __check(device->__glGetError());
                break;
            }
            default: std::unreachable();
        }
    }
    device->__glBindTexture(image->gl_target, 0);
    __check(device->__glGetError());

    device->__glPixelStorei(GL_UNPACK_ROW_LENGTH,   0);
    __check(device->__glGetError());
    device->__glPixelStorei(GL_UNPACK_IMAGE_HEIGHT, 0);
    __check(device->__glGetError());
    device->__glPixelStorei(GL_UNPACK_ALIGNMENT,    4);
    __check(device->__glGetError());
}

auto gfx::CreateSampler(Device device, SamplerCreateInfo const& create_info) -> Sampler {
    GLuint gl_sampler = 0;
    device->__glGenSamplers(1, &gl_sampler);
    __check(device->__glGetError());

    device->__glSamplerParameteri(gl_sampler, GL_TEXTURE_MIN_FILTER, __to_gl_min_filter(create_info.min_filter, create_info.mipmap_mode));
    __check(device->__glGetError());

    device->__glSamplerParameteri(gl_sampler, GL_TEXTURE_MAG_FILTER, __to_gl_mag_filter(create_info.mag_filter));
    __check(device->__glGetError());

    device->__glSamplerParameteri(gl_sampler, GL_TEXTURE_WRAP_S, __to_gl_wrap(create_info.address_mode_u));
    __check(device->__glGetError());

    device->__glSamplerParameteri(gl_sampler, GL_TEXTURE_WRAP_T, __to_gl_wrap(create_info.address_mode_v));
    __check(device->__glGetError());

    device->__glSamplerParameteri(gl_sampler, GL_TEXTURE_WRAP_R, __to_gl_wrap(create_info.address_mode_w));
    __check(device->__glGetError());

    device->__glSamplerParameterf(gl_sampler, GL_TEXTURE_LOD_BIAS, create_info.mip_lod_bias);
    __check(device->__glGetError());

    device->__glSamplerParameterf(gl_sampler, GL_TEXTURE_MIN_LOD, create_info.min_lod);
    __check(device->__glGetError());

    device->__glSamplerParameterf(gl_sampler, GL_TEXTURE_MAX_LOD, create_info.max_lod);
    __check(device->__glGetError());

    if (create_info.compare_enable) {
        device->__glSamplerParameteri(gl_sampler, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        device->__glSamplerParameteri(gl_sampler, GL_TEXTURE_COMPARE_FUNC, __to_gl_compare_func(create_info.compare_op));
    } else {
        device->__glSamplerParameteri(gl_sampler, GL_TEXTURE_COMPARE_MODE, GL_NONE);
    }
    if (create_info.anisotropy_enable) {
        device->__glSamplerParameterf(gl_sampler, GL_TEXTURE_MAX_ANISOTROPY_EXT, create_info.max_anisotropy);
        __check(device->__glGetError());
    }
    return new Sampler_T(gl_sampler);
}

void gfx::DeleteSampler(Device device, Sampler sampler) {
    device->__glDeleteSamplers(1, &sampler->gl_sampler);
    __check(device->__glGetError());

    delete sampler;
}

void gfx::Submit(Device device, CommandBuffer command_buffer) {
    std::uint32_t push_constant_buffer = {};
    void* push_constant_block = {};

    std::span<Buffer const> vertex_buffers = {};
    std::span<VertexBinding const> vertex_bindings = {};
    std::span<VertexAttribute const> vertex_attributes = {};

    if (command_buffer->push_constant_length > 0) {
        device->__glGenBuffers(1, &push_constant_buffer);
        __check(device->__glGetError());
        device->__glBindBuffer(GL_UNIFORM_BUFFER, push_constant_buffer);
        __check(device->__glGetError());
        device->__glBufferData(GL_UNIFORM_BUFFER, static_cast<i64>(command_buffer->push_constant_length), nullptr, GL_DYNAMIC_DRAW);
        __check(device->__glGetError());

        push_constant_block = device->__glMapBufferRange(
            GL_UNIFORM_BUFFER,
            0,
            static_cast<GLsizeiptr>(command_buffer->push_constant_length),
            GL_MAP_WRITE_BIT | GL_MAP_FLUSH_EXPLICIT_BIT | GL_MAP_UNSYNCHRONIZED_BIT
        );
        __check(device->__glGetError());

        device->__glBindBuffer(GL_UNIFORM_BUFFER, 0);
        __check(device->__glGetError());
    }
    for (const auto& [_, command] : command_buffer->commands) {
        match_(command) {
            case_(CmdBeginRenderPass const& cmd) {
                device->__glBindFramebuffer(GL_DRAW_FRAMEBUFFER, cmd.gl_framebuffer);
                __check(device->__glGetError());
                for (std::size_t i = 0; i < cmd.color_attachments.size(); i += 1) {
                    device->__glClearBufferfv(GL_COLOR, static_cast<GLint>(i), &cmd.color_attachments[i].clear_value.r);
                    __check(device->__glGetError());
                }
                if (cmd.has_depth_attachment) {
                    device->__glClearBufferfv(GL_DEPTH, 0, &cmd.depth_clear_value);
                    __check(device->__glGetError());
                }
                if (cmd.has_stencil_attachment) {
                    device->__glClearBufferiv(GL_STENCIL, 0, &cmd.stencil_clear_value);
                    __check(device->__glGetError());
                }
            },
            case_(CmdSetScissor const& cmd) {
                device->__glScissorIndexed(cmd.first_index, cmd.scissor.x, cmd.scissor.y, static_cast<GLsizei>(cmd.scissor.width), static_cast<GLsizei>(cmd.scissor.height));
                __check(device->__glGetError());
            },
            case_(CmdSetViewport const& cmd) {
                device->__glViewportIndexedf(cmd.first_index, cmd.viewport.x, cmd.viewport.y, cmd.viewport.width, cmd.viewport.height);
                __check(device->__glGetError());
            },
            case_(CmdSetRenderPipeline const& cmd) {
                vertex_bindings = cmd.pipeline->vertex_bindings;
                vertex_attributes = cmd.pipeline->vertex_attributes;

                device->__glUseProgramStages(command_buffer->gl_pipeline, GL_VERTEX_SHADER_BIT, cmd.pipeline->vertex_shader->gl_program);
                __check(device->__glGetError());
                device->__glUseProgramStages(command_buffer->gl_pipeline, GL_FRAGMENT_SHADER_BIT, cmd.pipeline->fragment_shader->gl_program);
                __check(device->__glGetError());
                device->__glBindProgramPipeline(command_buffer->gl_pipeline);
                __check(device->__glGetError());
                device->__glEnable(GL_SCISSOR_TEST);
                __check(device->__glGetError());
                switch (cmd.pipeline->cull_mode) {
                case CullMode::none:
                    device->__glDisable(GL_CULL_FACE);
                    __check(device->__glGetError());
                    break;
                case CullMode::front:
                    device->__glEnable(GL_CULL_FACE);
                    __check(device->__glGetError());
                    device->__glCullFace(GL_FRONT);
                    __check(device->__glGetError());
                    break;
                case CullMode::back:
                    device->__glEnable(GL_CULL_FACE);
                    __check(device->__glGetError());
                    device->__glCullFace(GL_BACK);
                    __check(device->__glGetError());
                    break;
                }
                for (std::size_t i = 0; i < cmd.pipeline->blend_attachments.size(); i += 1) {
                    if (cmd.pipeline->blend_attachments[i].enable) {
                        auto const src_rgb = __to_gl_blend_factor(cmd.pipeline->blend_attachments[i].src_color);
                        auto const dst_rgb = __to_gl_blend_factor(cmd.pipeline->blend_attachments[i].dst_color);
                        auto const src_alpha = __to_gl_blend_factor(cmd.pipeline->blend_attachments[i].src_alpha);
                        auto const dst_alpha = __to_gl_blend_factor(cmd.pipeline->blend_attachments[i].dst_alpha);
                        auto const mode_rgb = __to_gl_blend_op(cmd.pipeline->blend_attachments[i].color_op);
                        auto const mode_alpha = __to_gl_blend_op(cmd.pipeline->blend_attachments[i].alpha_op);
                        device->__glEnablei(GL_BLEND, i);
                        __check(device->__glGetError());
                        device->__glBlendFuncSeparatei(i, src_rgb, dst_rgb, src_alpha, dst_alpha);
                        __check(device->__glGetError());
                        device->__glBlendEquationSeparatei(i, mode_rgb, mode_alpha);
                        __check(device->__glGetError());
                    } else {
                        device->__glDisablei(GL_BLEND, i);
                        __check(device->__glGetError());
                    }
                }
                if (cmd.pipeline->is_depth_write_enabled || cmd.pipeline->depth_compare_op != CompareOp::always) {
                    device->__glEnable(GL_DEPTH_TEST);
                    __check(device->__glGetError());
                    device->__glDepthFunc(__to_gl_compare_func(cmd.pipeline->depth_compare_op));
                    __check(device->__glGetError());
                    device->__glDepthMask(cmd.pipeline->is_depth_write_enabled ? GL_TRUE : GL_FALSE);
                    __check(device->__glGetError());
                } else {
                    device->__glDisable(GL_DEPTH_TEST);
                    __check(device->__glGetError());
                }
            },
            case_(CmdSetVertexBuffers const& cmd) {
                vertex_buffers = cmd.buffers;
            },
            case_(CmdBindBuffer const& cmd) {
                device->__glBindBufferBase(GL_UNIFORM_BUFFER, cmd.binding, cmd.buffer->gl_buffer);
                __check(device->__glGetError());
            },
            case_(CmdBindCombinedImageSampler const& cmd) {
                device->__glActiveTexture(GL_TEXTURE0 + cmd.binding);
                __check(device->__glGetError());

                device->__glBindTexture(cmd.image->gl_target, cmd.image->gl_texture);
                __check(device->__glGetError());

                device->__glBindSampler(cmd.binding, cmd.sampler->gl_sampler);
                __check(device->__glGetError());
            },
            case_(CmdSetBytes const& cmd) {
                auto const length = __align_up(cmd.bytes.size(), device->uniform_buffer_offset_alignment);

                std::memcpy(
                    static_cast<std::byte*>(push_constant_block) + cmd.offset,
                    cmd.bytes.data(),
                    cmd.bytes.size()
                );

                device->__glBindBuffer(GL_UNIFORM_BUFFER, push_constant_buffer);
                __check(device->__glGetError());

                device->__glFlushMappedBufferRange(
                    GL_UNIFORM_BUFFER,
                    static_cast<GLintptr>(cmd.offset),
                    static_cast<GLsizeiptr>(length)
                );
                __check(device->__glGetError());

                device->__glBindBuffer(GL_UNIFORM_BUFFER, 0);
                __check(device->__glGetError());

                device->__glBindBufferRange(
                    GL_UNIFORM_BUFFER,
                    cmd.binding,
                    push_constant_buffer,
                    static_cast<GLintptr>(cmd.offset),
                    static_cast<GLsizeiptr>(length)
                );
                __check(device->__glGetError());
            },
            case_(CmdDraw const& cmd) {
                __bind_stencil_state(device, command_buffer);
                __bind_attributes(device, command_buffer, vertex_bindings, vertex_attributes, vertex_buffers);
                device->__glDrawArraysInstanced(
                    __to_gl_topology(cmd.topology),
                    static_cast<std::int32_t>(cmd.first_vertex),
                    static_cast<std::int32_t>(cmd.vertex_count),
                    static_cast<std::int32_t>(cmd.instance_count)
                );
                __check(device->__glGetError());
                __unbind_attributes(device, command_buffer, vertex_bindings, vertex_attributes, vertex_buffers);
            },
            case_(CmdDrawIndexed const& cmd) {
                __bind_stencil_state(device, command_buffer);
                __bind_attributes(device, command_buffer, vertex_bindings, vertex_attributes, vertex_buffers);
                device->__glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cmd.index_buffer->gl_buffer);
                __check(device->__glGetError());
                device->__glDrawElementsInstancedBaseVertex(
                    __to_gl_topology(cmd.topology),
                    static_cast<GLsizei>(cmd.index_count),
                    __to_gl_index_type(cmd.index_type),
                    reinterpret_cast<void*>(static_cast<std::uintptr_t>(cmd.index_offset)),
                    static_cast<GLsizei>(cmd.instance_count),
                    static_cast<GLint>(cmd.base_vertex)
                );
                __check(device->__glGetError());
                device->__glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_NONE);
                __check(device->__glGetError());
                __unbind_attributes(device, command_buffer, vertex_bindings, vertex_attributes, vertex_buffers);
            },
            case_(CmdSetStencilTestEnable const& cmd) {
                command_buffer->stencil_test_enable = cmd.stencil_test_enable;
            },
            case_(CmdSetStencilWriteMask const& cmd) {
                if ((cmd.face_mask == StencilFace::front) || (cmd.face_mask == StencilFace::front_and_back)) {
                    command_buffer->front_stencil_state.writeMask = cmd.write_mask;
                }
                if ((cmd.face_mask == StencilFace::back) || (cmd.face_mask == StencilFace::front_and_back)) {
                    command_buffer->back_stencil_state.writeMask  = cmd.write_mask;
                }
            },
            case_(CmdSetStencilCompareMask const& cmd) {
                if ((cmd.face_mask == StencilFace::front) || (cmd.face_mask == StencilFace::front_and_back)) {
                    command_buffer->front_stencil_state.compareMask = cmd.compare_mask;
                }
                if ((cmd.face_mask == StencilFace::back) || (cmd.face_mask == StencilFace::front_and_back)) {
                    command_buffer->back_stencil_state.compareMask  = cmd.compare_mask;
                }
            },
            case_(CmdSetStencilReference const& cmd) {
                if ((cmd.face_mask == StencilFace::front) || (cmd.face_mask == StencilFace::front_and_back)) {
                    command_buffer->front_stencil_state.reference = cmd.reference;
                }
                if ((cmd.face_mask == StencilFace::back) || (cmd.face_mask == StencilFace::front_and_back)) {
                    command_buffer->back_stencil_state.reference  = cmd.reference;
                }
            },
            case_(CmdSetStencilOp const& cmd) {
                if ((cmd.face_mask == StencilFace::front) || (cmd.face_mask == StencilFace::front_and_back)) {
                    command_buffer->front_stencil_state.failOp = cmd.fail_op;
                    command_buffer->front_stencil_state.passOp = cmd.pass_op;
                    command_buffer->front_stencil_state.depthFailOp = cmd.depth_fail_op;
                    command_buffer->front_stencil_state.compareOp = cmd.compare_op;
                }
                if ((cmd.face_mask == StencilFace::back) || (cmd.face_mask == StencilFace::front_and_back)) {
                    command_buffer->back_stencil_state.failOp = cmd.fail_op;
                    command_buffer->back_stencil_state.passOp = cmd.pass_op;
                    command_buffer->back_stencil_state.depthFailOp = cmd.depth_fail_op;
                    command_buffer->back_stencil_state.compareOp = cmd.compare_op;
                }
            },
            case_(CmdSetColorWriteMask const& cmd) {
                command_buffer->color_write_mask = cmd.color_write_mask;
            },
            case_(CmdClearStencil const& cmd) {
                device->__glClearBufferiv(GL_STENCIL, 0, &cmd.clear_value);
            }
        };
    }

    if (push_constant_buffer != 0) {
        device->__glBindBuffer(GL_UNIFORM_BUFFER, push_constant_buffer);
        __check(device->__glGetError());
        device->__glUnmapBuffer(GL_UNIFORM_BUFFER);
        __check(device->__glGetError());
        device->__glBindBuffer(GL_UNIFORM_BUFFER, 0);
        __check(device->__glGetError());
        device->__glDeleteBuffers(1, &push_constant_buffer);
        __check(device->__glGetError());
    }

    // device->__glBindFramebuffer(GL_DRAW_FRAMEBUFFER, GL_NONE);
    // __check(device->__glGetError());

    // GLsync gl_sync = device->__glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
    // __check(device->__glGetError());
}

auto gfx::CreateCommandBuffer(Device device) -> CommandBuffer {
    auto* command_buffer = new CommandBuffer_T();
    command_buffer->uniform_buffer_offset_alignment = device->uniform_buffer_offset_alignment;

    device->__glGenProgramPipelines(1, &command_buffer->gl_pipeline);
    __check(device->__glGetError());

    device->__glGenVertexArrays(1, &command_buffer->gl_vertex_array);
    __check(device->__glGetError());

    StencilOpState stencil_op_state = {};
    stencil_op_state.failOp = StencilOp::keep;
    stencil_op_state.passOp = StencilOp::keep;
    stencil_op_state.depthFailOp = StencilOp::keep;
    stencil_op_state.compareOp = CompareOp::always;
    stencil_op_state.compareMask = std::numeric_limits<std::uint32_t>::max();
    stencil_op_state.writeMask = std::numeric_limits<std::uint32_t>::max();
    stencil_op_state.reference = 0;

    command_buffer->stencil_test_enable = false;
    command_buffer->back_stencil_state = stencil_op_state;
    command_buffer->front_stencil_state = stencil_op_state;
    command_buffer->color_write_mask = ColorWriteMask(true, true, true, true);
    return command_buffer;
}

void gfx::DeleteCommandBuffer(Device device, CommandBuffer command_buffer) {
    device->__glDeleteProgramPipelines(1, &command_buffer->gl_pipeline);
    __check(device->__glGetError());

    device->__glDeleteVertexArrays(1, &command_buffer->gl_vertex_array);
    __check(device->__glGetError());

    delete command_buffer;
}

void gfx::ResetCommandBuffer(Device device, CommandBuffer command_buffer) {
    command_buffer->commands.clear();
    command_buffer->push_constant_length = 0;
}

void gfx::BeginRenderPass(CommandBuffer command_buffer, RenderPassBeginInfo const& begin_info) {
    CmdBeginRenderPass cmd = {};
    cmd.gl_framebuffer = begin_info.framebuffer;
    cmd.color_attachments.assign_range(begin_info.color_attachments);
    cmd.has_depth_attachment = begin_info.depth_attachment.has_value();
    if (begin_info.depth_attachment.has_value()) {
        cmd.depth_clear_value = begin_info.depth_attachment->clear_value;
    }
    cmd.has_stencil_attachment = begin_info.stencil_attachment.has_value();
    if (begin_info.stencil_attachment.has_value()) {
        cmd.stencil_clear_value = begin_info.stencil_attachment->clear_value;
    }
    command_buffer->commands.emplace_back(CMD_TYPE_BEGIN_RENDER_PASS, std::move(cmd));
}

void gfx::EndRenderPass(CommandBuffer command_buffer) {

}

void gfx::SetScissor(CommandBuffer command_buffer, std::uint32_t first_index, Rect2D const scissor) {
    CmdSetScissor cmd = {};
    cmd.first_index = first_index;
    cmd.scissor = scissor;
    command_buffer->commands.emplace_back(CMD_TYPE_SET_SCISSOR, std::move(cmd));
}

void gfx::SetViewport(CommandBuffer command_buffer, std::uint32_t first_index, Viewport const viewport) {
    CmdSetViewport cmd = {};
    cmd.first_index = first_index;
    cmd.viewport = viewport;
    command_buffer->commands.emplace_back(CMD_TYPE_SET_VIEWPORT, std::move(cmd));
}

void gfx::SetRenderPipeline(CommandBuffer command_buffer, RenderPipeline pipeline) {
    CmdSetRenderPipeline cmd = {};
    cmd.pipeline = pipeline;
    command_buffer->commands.emplace_back(CMD_TYPE_SET_RENDER_PIPELINE, cmd);
}

void gfx::SetVertexBuffers(CommandBuffer command_buffer, std::span<Buffer const> buffers) {
    CmdSetVertexBuffers cmd = {};
    cmd.buffers.assign_range(buffers);
    command_buffer->commands.emplace_back(CMD_TYPE_SET_VERTEX_BUFFERS, std::move(cmd));
}

void gfx::BindBuffer(CommandBuffer command_buffer, std::uint32_t binding, Buffer buffer) {
    CmdBindBuffer cmd = {};
    cmd.binding = binding;
    cmd.buffer = buffer;
    command_buffer->commands.emplace_back(CMD_TYPE_BIND_BUFFER, cmd);
}

void gfx::BindCombinedImageSampler(CommandBuffer command_buffer, std::uint32_t binding, Image image, Sampler sampler) {
    CmdBindCombinedImageSampler cmd = {};
    cmd.binding = binding;
    cmd.image = image;
    cmd.sampler = sampler;
    command_buffer->commands.emplace_back(CMD_TYPE_BIND_COMBINED_IMAGE_SAMPLER, cmd);
}

void gfx::SetBytes(CommandBuffer command_buffer, std::uint32_t binding, std::uint32_t size, void const* data) {
    std::size_t const offset = command_buffer->push_constant_length;
    command_buffer->push_constant_length += __align_up(size, command_buffer->uniform_buffer_offset_alignment);

    CmdSetBytes cmd = {};
    cmd.binding = binding;
    cmd.offset = offset;
    cmd.bytes.assign_range(std::span(static_cast<std::byte const*>(data), size));
    command_buffer->commands.emplace_back(CMD_TYPE_SET_BYTES, std::move(cmd));
}

void gfx::Draw(CommandBuffer command_buffer, Topology topology, std::uint32_t vertex_count, std::uint32_t first_vertex, std::uint32_t instance_count) {
    CmdDraw cmd = {};
    cmd.topology = topology;
    cmd.vertex_count = vertex_count;
    cmd.first_vertex = first_vertex;
    cmd.instance_count = instance_count;
    command_buffer->commands.emplace_back(CMD_TYPE_DRAW, cmd);
}

void gfx::DrawIndexed(CommandBuffer command_buffer, Topology topology, Buffer index_buffer, std::uint32_t index_offset, std::uint32_t index_count, std::uint32_t base_vertex, IndexType index_type, std::uint32_t instance_count) {
    CmdDrawIndexed cmd = {};
    cmd.topology = topology;
    cmd.index_buffer = index_buffer;
    cmd.index_offset = index_offset;
    cmd.index_count = index_count;
    cmd.base_vertex = base_vertex;
    cmd.index_type = index_type;
    cmd.instance_count = instance_count;
    command_buffer->commands.emplace_back(CMD_TYPE_DRAW_INDEXED, cmd);
}

void gfx::SetStencilTestEnable(CommandBuffer command_buffer, bool stencil_test_enable) {
    CmdSetStencilTestEnable cmd = {};
    cmd.stencil_test_enable = stencil_test_enable;
    command_buffer->commands.emplace_back(CMD_TYPE_SET_STENCIL_TEST_ENABLE, cmd);
}

void gfx::SetStencilWriteMask(CommandBuffer command_buffer, StencilFace face_mask, std::uint32_t write_mask) {
    CmdSetStencilWriteMask cmd = {};
    cmd.face_mask = face_mask;
    cmd.write_mask = write_mask;
    command_buffer->commands.emplace_back(CMD_TYPE_SET_STENCIL_WRITE_MASK, cmd);
}

void gfx::SetStencilCompareMask(CommandBuffer command_buffer, StencilFace face_mask, std::uint32_t compare_mask) {
    CmdSetStencilCompareMask cmd = {};
    cmd.face_mask = face_mask;
    cmd.compare_mask = compare_mask;
    command_buffer->commands.emplace_back(CMD_TYPE_SET_STENCIL_COMPARE_MASK, cmd);
}

void gfx::SetStencilReference(CommandBuffer command_buffer, StencilFace face_mask, std::uint32_t reference) {
    CmdSetStencilReference cmd = {};
    cmd.face_mask = face_mask;
    cmd.reference = reference;
    command_buffer->commands.emplace_back(CMD_TYPE_SET_STENCIL_REFERENCE, cmd);
}

void gfx::SetStencilOp(CommandBuffer command_buffer, StencilFace face_mask, StencilOp fail_op, StencilOp pass_op, StencilOp depth_fail_op, CompareOp compare_op) {
    CmdSetStencilOp cmd = {};
    cmd.face_mask = face_mask;
    cmd.fail_op = fail_op;
    cmd.pass_op = pass_op;
    cmd.depth_fail_op = depth_fail_op;
    cmd.compare_op = compare_op;
    command_buffer->commands.emplace_back(CMD_TYPE_SET_STENCIL_OP, cmd);
}

void gfx::SetColorWriteMask(CommandBuffer command_buffer, ColorWriteMask color_write_mask) {
    CmdSetColorWriteMask cmd = {};
    cmd.color_write_mask = color_write_mask;
    command_buffer->commands.emplace_back(CMD_TYPE_SET_COLOR_WRITE_MASK, cmd);
}

void gfx::ClearStencil(CommandBuffer command_buffer, std::int32_t clear_value) {
    CmdClearStencil cmd = {};
    cmd.clear_value = clear_value;
    command_buffer->commands.emplace_back(CMD_TYPE_CLEAR_STENCIL, cmd);
}