#include "core/application.hpp"

#include <chrono>

namespace core {
auto Application::initialize() -> bool {
    if (!window_.initialize()) {
        return false;
    }
    if (!renderer_.initialize()) {
        window_.shutdown();
        return false;
    }

    running_ = true;
    return true;
}

auto Application::run() -> int {
    if (!initialize()) {
        return 1;
    }

    auto previous_time = std::chrono::steady_clock::now();
    while (running_) {
        auto const now = std::chrono::steady_clock::now();
        float const delta_time = std::chrono::duration<float>(now - previous_time).count();
        previous_time = now;

        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running_ = false;
            } else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
                running_ = false;
            } else {
                controller_.handle_event(event, camera_);
            }
        }

        controller_.update(camera_, delta_time);
        world_.ensure_region(camera_.position(), 12);

        int width = 0;
        int height = 0;
        window_.size(width, height);
        renderer_.render(world_, camera_, width, height);
        window_.swap();
    }

    shutdown();
    return 0;
}

void Application::shutdown() {
    renderer_.shutdown(world_);
    window_.shutdown();
}
}
