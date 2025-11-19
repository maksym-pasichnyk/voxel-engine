#pragma once

#include <GL/glew.h>

#include <string_view>

namespace render {
class ShaderProgram {
public:
    ShaderProgram() = default;
    ~ShaderProgram() = default;

    auto create(std::string_view vertex_source, std::string_view fragment_source) -> bool;
    void destroy();
    void use() const;
    auto id() const -> GLuint { return program_; }

private:
    auto compile(GLenum type, std::string_view source) -> GLuint;

    GLuint program_ = 0;
};
}
