#include "scene/fps_camera.hpp"

#include <algorithm>

#include "Mathematics.hpp"

namespace scene {
FpsCamera::FpsCamera() {
    set_position(FVector3(0.0f, 48.0f, -12.0f));
}

void FpsCamera::set_position(FVector3 const& position) {
    position_ = position;
}

void FpsCamera::set_move_speed(float move_speed) {
    move_speed_ = move_speed;
}

void FpsCamera::set_mouse_sensitivity(float mouse_sensitivity) {
    mouse_sensitivity_ = mouse_sensitivity;
}

void FpsCamera::rotate(float yaw_delta, float pitch_delta) {
    yaw_ += yaw_delta * mouse_sensitivity_;
    pitch_ = std::clamp(pitch_ + pitch_delta * mouse_sensitivity_, -90.0f, 90.0f);
}

void FpsCamera::move(FVector3 const& local_input, float delta_time) {
    FVector3 const move_direction = view_matrix().TransformDirection(local_input);
    position_ += move_direction * move_speed_ * delta_time;
}

auto FpsCamera::position() const -> FVector3 const& {
    return position_;
}

auto FpsCamera::forward() const -> FVector3 {
    return view_matrix().TransformDirection(FVector3(0.0f, 0.0f, 1.0f));
}

auto FpsCamera::world_matrix() const -> FMatrix4x4 {
    return FMatrix4x4::Identity().Translate(position_).Rotate(math::DegreesToRadians(FVector3(pitch_, yaw_, 0.0f)));
}

auto FpsCamera::view_matrix() const -> FMatrix4x4 {
    return world_matrix().Invert();
}
}
