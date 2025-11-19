#pragma once

#include "FMatrix4x4.hpp"
#include "FVector3.hpp"

namespace scene {
class FpsCamera {
public:
    FpsCamera();

    void set_position(FVector3 const& position);
    void set_move_speed(float move_speed);
    void set_mouse_sensitivity(float mouse_sensitivity);

    void rotate(float yaw_delta, float pitch_delta);
    void move(FVector3 const& local_input, float delta_time);

    auto position() const -> FVector3 const&;
    auto forward() const -> FVector3;
    auto world_matrix() const -> FMatrix4x4;
    auto view_matrix() const -> FMatrix4x4;

private:
    FVector3 position_ = FVector3(0.0f, 0.0f, 0.0f);
    float yaw_ = 0.0f;
    float pitch_ = 0.0f;
    float move_speed_ = 18.0f;
    float mouse_sensitivity_ = 0.12f;
};
}
