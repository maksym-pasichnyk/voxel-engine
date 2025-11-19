#pragma once

#include <SDL3/SDL.h>

#include "scene/fps_camera.hpp"

namespace game {
class PlayerController {
public:
    void handle_event(SDL_Event const& event, scene::FpsCamera& camera);
    void update(scene::FpsCamera& camera, float delta_time) const;
};
}
