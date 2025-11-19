#include "game/player_controller.hpp"

#include "FVector3.hpp"

namespace game {
void PlayerController::handle_event(SDL_Event const& event, scene::FpsCamera& camera) {
    if (event.type == SDL_EVENT_MOUSE_MOTION) {
        camera.rotate(static_cast<float>(event.motion.xrel), static_cast<float>(event.motion.yrel));
    }
}

void PlayerController::update(scene::FpsCamera& camera, float delta_time) const {
    bool const* keyboard = SDL_GetKeyboardState(nullptr);
    FVector3 local_input(
        static_cast<float>((keyboard[SDL_SCANCODE_D] ? 1 : 0) - (keyboard[SDL_SCANCODE_A] ? 1 : 0)),
        static_cast<float>((keyboard[SDL_SCANCODE_SPACE] ? 1 : 0) - (keyboard[SDL_SCANCODE_LSHIFT] ? 1 : 0)),
        static_cast<float>((keyboard[SDL_SCANCODE_W] ? 1 : 0) - (keyboard[SDL_SCANCODE_S] ? 1 : 0))
    );
    camera.move(local_input, delta_time);
}
}
