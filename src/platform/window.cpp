#include "platform/window.hpp"

#include <GL/glew.h>

#include <print>

namespace platform {
auto Window::initialize() -> bool {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::println(stderr, "SDL_Init failed: {}", SDL_GetError());
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    window_ = SDL_CreateWindow(
        "VoxelEngine Reset",
        1280,
        720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY
    );
    if (window_ == nullptr) {
        std::println(stderr, "SDL_CreateWindow failed: {}", SDL_GetError());
        SDL_Quit();
        return false;
    }

    gl_context_ = SDL_GL_CreateContext(window_);
    if (gl_context_ == nullptr) {
        std::println(stderr, "SDL_GL_CreateContext failed: {}", SDL_GetError());
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        SDL_Quit();
        return false;
    }

    SDL_GL_SetSwapInterval(1);
    SDL_SetWindowRelativeMouseMode(window_, true);

    glewExperimental = GL_TRUE;
    if (GLenum const error = glewInit(); error != GLEW_OK) {
        std::println(stderr, "glewInit failed: {}", reinterpret_cast<char const*>(glewGetErrorString(error)));
        shutdown();
        return false;
    }

    while (glGetError() != GL_NO_ERROR) {}
    return true;
}

void Window::shutdown() {
    if (gl_context_ != nullptr) {
        SDL_GL_DestroyContext(gl_context_);
        gl_context_ = nullptr;
    }
    if (window_ != nullptr) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
}

void Window::swap() const {
    SDL_GL_SwapWindow(window_);
}

void Window::size(int& width, int& height) const {
    SDL_GetWindowSizeInPixels(window_, &width, &height);
}
}
