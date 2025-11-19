#pragma once

#include "game/player_controller.hpp"
#include "platform/window.hpp"
#include "render/voxel_renderer.hpp"
#include "scene/fps_camera.hpp"
#include "world/voxel_world.hpp"

namespace core {
class Application {
public:
    auto run() -> int;

private:
    auto initialize() -> bool;
    void shutdown();

    platform::Window window_;
    scene::FpsCamera camera_;
    game::PlayerController controller_;
    voxel::VoxelWorld world_;
    render::VoxelRenderer renderer_;
    bool running_ = false;
};
}
