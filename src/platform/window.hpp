#pragma once

#include <SDL3/SDL.h>

namespace platform {
class Window {
public:
    Window() = default;
    ~Window() = default;

    auto initialize() -> bool;
    void shutdown();
    void swap() const;
    void size(int& width, int& height) const;

    auto handle() const -> SDL_Window* { return window_; }

private:
    SDL_Window* window_ = nullptr;
    SDL_GLContext gl_context_ = nullptr;
};
}
