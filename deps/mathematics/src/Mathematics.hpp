//
// Created by Maksym Pasichnyk on 15.06.2025.
//
#pragma once

#include "FVector2I.hpp"
#include "FVector3I.hpp"
#include "FVector4.hpp"
#include "FVector4D.hpp"
#include "FMatrix4x4.hpp"

namespace math {
    template<typename T>
    static constexpr std::floating_point auto PI = static_cast<T>(3.14159265358979323846264338327950288);

    template<typename T>
    static constexpr std::floating_point auto RAD_2_DEG = static_cast<T>(57.29577951308232);

    template<typename T>
    static constexpr std::floating_point auto DEG_2_RAD = static_cast<T>(0.017453292519943295);

    template<std::floating_point T>
    static constexpr auto RadiansToDegrees(T const Radians) -> T {
        return Radians * RAD_2_DEG<T>;
    }

    template<std::floating_point T>
    static constexpr auto DegreesToRadians(T const Degrees) -> T {
        return Degrees * DEG_2_RAD<T>;
    }

    static constexpr auto DegreesToRadians(FVector3 const Degrees) -> FVector3 {
        return Degrees * DEG_2_RAD<std::float_t>;
    }

    static constexpr auto CreatePerspectiveFieldOfView(std::float_t const Fov, std::float_t const Aspect, std::float_t const NearZ, std::float_t const FarZ) -> FMatrix4x4 {
        auto const r = 1.0f / std::tan(Fov * 0.5f);
        auto const x = r / Aspect;
        auto const y = r;
        auto const z = (NearZ + FarZ) / (FarZ - NearZ);
        auto const a = 1.0f;
        auto const b = -2.0f * NearZ * FarZ / (FarZ - NearZ);
        return FMatrix4x4{
            FVector4(x, 0, 0, 0),
            FVector4(0, y, 0, 0),
            FVector4(0, 0, z, a),
            FVector4(0, 0, b, 0),
        };
        
        // auto const f = 1.0f / std::tan(Fov / 2.0f);
        // auto const a = f / Aspect;
        // auto const b = -f;
        // auto const c = FarZ / (NearZ - FarZ);
        // auto const d = (FarZ * NearZ) / (NearZ - FarZ);
        // // auto const c = (FarZ + NearZ) / (NearZ - FarZ);
        // // auto const d = (2 * FarZ * NearZ) / (NearZ - FarZ);
        // auto const e = -1.0f;
        //
        // return FMatrix4x4{
        //     FVector4(a, 0, 0, 0),
        //     FVector4(0, b, 0, 0),
        //     FVector4(0, 0, c, e),
        //     FVector4(0, 0, d, 1)
        // };
    }

    static constexpr auto CreateOrthographicProjection(std::float_t const left, std::float_t const right, std::float_t const bottom, std::float_t const top) -> FMatrix4x4 {
        auto const x = 2.0f / (right - left);
        auto const y = 2.0f / (top - bottom);
        auto const a = (right + left)/(left - right);
        auto const b = (top + bottom)/(bottom - top);
        return FMatrix4x4{
            FVector4(x, 0, 0, 0),
            FVector4(0, y, 0, 0),
            FVector4(0, 0, 1, 0),
            FVector4(a, b, 0, 1)
        };
    }

    static constexpr auto TransformRotationScale(FVector3 const& Translation, FVector3 const& Rotation, FVector3 const& Scale) -> FMatrix4x4 {
        auto const r = Rotation * FVector3(DEG_2_RAD<std::float_t>);
        auto const s = r.Sin();
        auto const c = r.Cos();

        auto const m00 = c.y * c.z;
        auto const m01 = c.y * s.z;
        auto const m02 = -s.y;
        auto const m10 = c.z * s.x * s.y - c.x * s.z;
        auto const m11 = c.x * c.z + s.x * s.y * s.z;
        auto const m12 = c.y * s.x;
        auto const m20 = c.x * c.z * s.y + s.x * s.z;
        auto const m21 = c.x * s.y * s.z - s.x * c.z;
        auto const m22 = c.x * c.y;

        auto const VecX = FVector3(m00, m01, m02) * Scale;
        auto const VecY = FVector3(m10, m11, m12) * Scale;
        auto const VecZ = FVector3(m20, m21, m22) * Scale;
        auto const VecW = Translation;

        return FMatrix4x4{
            FVector4(VecX, 0),
            FVector4(VecY, 0),
            FVector4(VecZ, 0),
            FVector4(VecW, 1)
        };
    }

    static constexpr auto ToIntVector(FVector3 const& This) -> FVector3I {
        return FVector3I(
            static_cast<std::int32_t>(This.x),
            static_cast<std::int32_t>(This.y),
            static_cast<std::int32_t>(This.z)
        );
    }

    static constexpr auto ToFloatVector(FVector2I const& This) -> FVector2 {
        return FVector2(
            static_cast<std::float_t>(This.x),
            static_cast<std::float_t>(This.y)
        );
    }

    static constexpr auto ToFloatVector(FVector2D const& This) -> FVector2 {
        return FVector2(
            static_cast<std::float_t>(This.x),
            static_cast<std::float_t>(This.y)
        );
    }

    static constexpr auto ToFloatVector(FVector3I const& This) -> FVector3 {
        return FVector3(
            static_cast<std::float_t>(This.x),
            static_cast<std::float_t>(This.y),
            static_cast<std::float_t>(This.z)
        );
    }

    static constexpr auto ToFloatVector(FVector3D const& This) -> FVector3 {
        return FVector3(
            static_cast<std::float_t>(This.x),
            static_cast<std::float_t>(This.y),
            static_cast<std::float_t>(This.z)
        );
    }

    // static constexpr auto ToFloatVector(FVector4I const& This) -> FVector4 {
    //     return FVector4(
    //         static_cast<std::float_t>(This.x),
    //         static_cast<std::float_t>(This.y),
    //         static_cast<std::float_t>(This.z),
    //         static_cast<std::float_t>(This.w)
    //     );
    // }

    static constexpr auto ToFloatVector(FVector4D const& This) -> FVector4 {
        return FVector4(
            static_cast<std::float_t>(This.x),
            static_cast<std::float_t>(This.y),
            static_cast<std::float_t>(This.z),
            static_cast<std::float_t>(This.w)
        );
    }

    static constexpr auto ToDoubleVector(FVector2 const& This) -> FVector2D {
        return FVector2D(This.x, This.y);
    }

    static constexpr auto ToDoubleVector(FVector3 const& This) -> FVector3D {
        return FVector3D(This.x, This.y, This.z);
    }

    static constexpr auto ToDoubleVector(FVector4 const& This) -> FVector4D {
        return FVector4D(This.x, This.y, This.z, This.w);
    }

    static constexpr auto GridSnap(FVector2 const& This, FVector2 const& Grid) -> FVector2 {
        return (This / Grid).Floor() * Grid;
    }

    static constexpr auto GridSnap(FVector3 const& This, FVector3 const& Grid) -> FVector3 {
        return (This / Grid).Floor() * Grid;
    }

    static constexpr auto GridSnap(FVector4 const& This, FVector4 const& Grid) -> FVector4 {
        return (This / Grid).Floor() * Grid;
    }

    static constexpr auto GridSnap(FVector2D const& This, FVector2D const& Grid) -> FVector2D {
        return (This / Grid).Floor() * Grid;
    }

    static constexpr auto GridSnap(FVector3D const& This, FVector3D const& Grid) -> FVector3D {
        return (This / Grid).Floor() * Grid;
    }

    static constexpr auto GridSnap(FVector4D const& This, FVector4D const& Grid) -> FVector4D {
        return (This / Grid).Floor() * Grid;
    }

    static constexpr auto SmoothDamp(FVector3 const& Current, FVector3 const& Target, FVector3& Velocity, std::float_t const SmoothTime, std::float_t const DeltaTime) -> FVector3
    {
        std::float_t const omega = 2.0f / SmoothTime;
        std::float_t const x = omega * DeltaTime;
        std::float_t const exp = 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
        FVector3 const change = Current - Target;
        FVector3 const temp = (Velocity + omega * change) * DeltaTime;
        Velocity = (Velocity - omega * temp) * exp;
        return Target + (change + temp) * exp;
    }

    static constexpr auto SmoothDamp(FVector3D const& Current, FVector3D const& Target, FVector3D& Velocity, std::double_t const SmoothTime, std::double_t const DeltaTime) -> FVector3D
    {
        std::double_t const omega = 2.0 / SmoothTime;
        std::double_t const x = omega * DeltaTime;
        std::double_t const exp = 1.0 / (1.0 + x + 0.48 * x * x + 0.235 * x * x * x);
        FVector3D const change = Current - Target;
        FVector3D const temp = (Velocity + omega * change) * DeltaTime;
        Velocity = (Velocity - omega * temp) * exp;
        return Target + (change + temp) * exp;
    }

    // static constexpr auto CreateTranslation(Vector3 const& Translation) -> Matrix4x4 {
    //     return Matrix4x4{
    //         Vector4(1, 0, 0, 0),
    //         Vector4(0, 1, 0, 0),
    //         Vector4(0, 0, 1, 0),
    //         Vector4(Translation, 1),
    //     };
    // }

    // static constexpr auto CreateRotation(Vector3 const& Rotation) -> Matrix4x4 {
    //     auto const r = Rotation * Vector3(DEG_2_RAD<std::float_t>);
    //     auto const s = MathHelper::Sin(r);
    //     auto const c = MathHelper::Cos(r);
    //     auto const m00 = c.y * c.z;
    //     auto const m10 = c.y * s.z;
    //     auto const m20 = -s.y;
    //     auto const m30 = 0;
    //     auto const m01 = c.z * s.x * s.y - c.x * s.z;
    //     auto const m11 = c.x * c.z + s.x * s.y * s.z;
    //     auto const m21 = c.y * s.x;
    //     auto const m31 = 0;
    //     auto const m02 = c.x * c.z * s.y + s.x * s.z;
    //     auto const m12 = c.x * s.y * s.z - s.x * c.z;
    //     auto const m22 = c.x * c.y;
    //     auto const m32 = 0.0f;
    //     auto const m03 = 0.0f;
    //     auto const m13 = 0.0f;
    //     auto const m23 = 0.0f;
    //     auto const m33 = 1.0f;
    //
    //     return Matrix4x4{
    //         Vector4(m00, m10, m20, m30),
    //         Vector4(m01, m11, m21, m31),
    //         Vector4(m02, m12, m22, m32),
    //         Vector4(m03, m13, m23, m33)
    //     };
    // }
    //
    // static constexpr auto CreateScale(Vector3 const& Scale) -> Matrix4x4 {
    //     return Matrix4x4{
    //         Vector4(Scale.x, 0, 0, 0),
    //         Vector4(0, Scale.y, 0, 0),
    //         Vector4(0, 0, Scale.z, 0),
    //         Vector4(0, 0, 0, 1),
    //     };
    // }

    static constexpr auto LookAt(FVector3 const Eye, FVector3 const Center, FVector3 const Up) -> FMatrix4x4 {
        auto const f = (Center - Eye).Normalized();
        auto const s = f.Cross(Up).Normalized();
        auto const u = s.Cross(f);

        return FMatrix4x4{
            FVector4(s.x, u.x, -f.x, 0),
            FVector4(s.y, u.y, -f.y, 0),
            FVector4(s.z, u.z, -f.z, 0),
            FVector4(-s.Dot(Eye), -u.Dot(Eye), f.Dot(Eye), 1)
        };
    }
};
