#include "render/shader_program.hpp"

#include <array>
#include <print>

namespace render {
auto ShaderProgram::compile(GLenum type, std::string_view source) -> GLuint {
    GLuint const shader = glCreateShader(type);
    char const* source_ptr = source.data();
    GLint const length = static_cast<GLint>(source.size());
    glShaderSource(shader, 1, &source_ptr, &length);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_TRUE) {
        return shader;
    }

    std::array<char, 1024> log = {};
    GLsizei written = 0;
    glGetShaderInfoLog(shader, static_cast<GLsizei>(log.size()), &written, log.data());
    std::println(stderr, "Shader compilation failed: {}", log.data());
    glDeleteShader(shader);
    return 0;
}

auto ShaderProgram::create(std::string_view vertex_source, std::string_view fragment_source) -> bool {
    GLuint const vertex = compile(GL_VERTEX_SHADER, vertex_source);
    GLuint const fragment = compile(GL_FRAGMENT_SHADER, fragment_source);
    if (vertex == 0 || fragment == 0) {
        if (vertex != 0) {
            glDeleteShader(vertex);
        }
        if (fragment != 0) {
            glDeleteShader(fragment);
        }
        return false;
    }

    program_ = glCreateProgram();
    glAttachShader(program_, vertex);
    glAttachShader(program_, fragment);
    glLinkProgram(program_);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint success = GL_FALSE;
    glGetProgramiv(program_, GL_LINK_STATUS, &success);
    if (success == GL_TRUE) {
        return true;
    }

    std::array<char, 1024> log = {};
    GLsizei written = 0;
    glGetProgramInfoLog(program_, static_cast<GLsizei>(log.size()), &written, log.data());
    std::println(stderr, "Program link failed: {}", log.data());
    destroy();
    return false;
}

void ShaderProgram::destroy() {
    if (program_ != 0) {
        glDeleteProgram(program_);
        program_ = 0;
    }
}

void ShaderProgram::use() const {
    glUseProgram(program_);
}
}
