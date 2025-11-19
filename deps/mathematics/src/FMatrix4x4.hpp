//
// Created by Maksym Pasichnyk on 15.06.2025.
//
#pragma once

#include "FMatrix3x3.hpp"
#include "FVector4.hpp"

struct FMatrix4x4 {
    FVector4 Columns[4] = {};

    static constexpr auto Identity() -> FMatrix4x4 {
        return FMatrix4x4{
           FVector4(1, 0, 0, 0),
           FVector4(0, 1, 0, 0),
           FVector4(0, 0, 1, 0),
           FVector4(0, 0, 0, 1),
        };
    }

    constexpr explicit operator FMatrix3x3(this FMatrix4x4 const& self) {
        return FMatrix3x3{
            static_cast<FVector3>(self.Columns[0]),
            static_cast<FVector3>(self.Columns[1]),
            static_cast<FVector3>(self.Columns[2]),
        };
    }

    constexpr auto Transpose(this FMatrix4x4 const& self) -> FMatrix4x4 {
        return FMatrix4x4{
            FVector4(self.Columns[0][0], self.Columns[1][0], self.Columns[2][0], self.Columns[3][0]),
            FVector4(self.Columns[0][1], self.Columns[1][1], self.Columns[2][1], self.Columns[3][1]),
            FVector4(self.Columns[0][2], self.Columns[1][2], self.Columns[2][2], self.Columns[3][2]),
            FVector4(self.Columns[0][3], self.Columns[1][3], self.Columns[2][3], self.Columns[3][3]),
        };
    }

    constexpr auto Invert(this FMatrix4x4 const& self) -> FMatrix4x4 {
        FVector4 a(self[0, 0], self[0, 1], self[0, 2], self[0, 3]);
        FVector4 b(self[1, 0], self[1, 1], self[1, 2], self[1, 3]);
        FVector4 c(self[2, 0], self[2, 1], self[2, 2], self[2, 3]);
        FVector4 d(self[3, 0], self[3, 1], self[3, 2], self[3, 3]);

        FVector4 c2211(c.z, c.z, c.y, c.y);
        FVector4 b2211(b.z, b.z, b.y, b.y);
        FVector4 a2211(a.z, a.z, a.y, a.y);
        FVector4 d3332(d.w, d.w, d.w, d.z);
        FVector4 c3332(c.w, c.w, c.w, c.z);
        FVector4 b3332(b.w, b.w, b.w, b.z);
        FVector4 d2211(d.z, d.z, d.y, d.y);
        FVector4 a3332(a.w, a.w, a.w, a.z);
        FVector4 b1000(b.y, b.x, b.x, b.x);
        FVector4 a1000(a.y, a.x, a.x, a.x);
        FVector4 c1000(c.y, c.x, c.x, c.x);
        FVector4 d1000(d.y, d.x, d.x, d.x);

        FVector4 v1 = c2211 * d3332 - d2211 * c3332;
        FVector4 v2 = b2211 * d3332 - d2211 * b3332;
        FVector4 v3 = b2211 * c3332 - c2211 * b3332;
        FVector4 v4 = a2211 * d3332 - d2211 * a3332;
        FVector4 v5 = a2211 * c3332 - c2211 * a3332;
        FVector4 v6 = a2211 * b3332 - b2211 * a3332;

        FVector4 c0 = (b1000 * v1 - c1000 * v2 + d1000 * v3) * FVector4(+1, -1, +1, -1);
        FVector4 c1 = (a1000 * v1 - c1000 * v4 + d1000 * v5) * FVector4(-1, +1, -1, +1);
        FVector4 c2 = (a1000 * v2 - b1000 * v4 + d1000 * v6) * FVector4(+1, -1, +1, -1);
        FVector4 c3 = (a1000 * v3 - b1000 * v5 + c1000 * v6) * FVector4(-1, +1, -1, +1);

        std::float_t invDet = 1.0f / a.Dot(c0);
        return FMatrix4x4{
            c0 * invDet,
            c1 * invDet,
            c2 * invDet,
            c3 * invDet
        };
    }

    constexpr auto Translate(this FMatrix4x4 const& self, FVector3 const& Translation) -> FMatrix4x4 {
        FMatrix4x4 const TranslationMatrix = {
            FVector4(1, 0, 0, 0),
            FVector4(0, 1, 0, 0),
            FVector4(0, 0, 1, 0),
            FVector4(Translation, 1)
        };
        return self * TranslationMatrix;
    }

    constexpr auto Rotate(this FMatrix4x4 const& self, FVector3 const& RadianEulerAngles) -> FMatrix4x4 {
        auto const s = RadianEulerAngles.Sin();
        auto const c = RadianEulerAngles.Cos();
        auto const m00 = c[1] * c[2];
        auto const m10 = c[1] * s[2];
        auto const m20 = -s[1];
        auto const m30 = 0;
        auto const m01 = c[2] * s[0] * s[1] - c[0] * s[2];
        auto const m11 = c[0] * c[2] + s[0] * s[1] * s[2];
        auto const m21 = c[1] * s[0];
        auto const m31 = 0;
        auto const m02 = c[0] * c[2] * s[1] + s[0] * s[2];
        auto const m12 = c[0] * s[1] * s[2] - s[0] * c[2];
        auto const m22 = c[0] * c[1];
        auto const m32 = 0.0f;
        auto const m03 = 0.0f;
        auto const m13 = 0.0f;
        auto const m23 = 0.0f;
        auto const m33 = 1.0f;

        FMatrix4x4 const RotationMatrix = {
            FVector4(m00, m10, m20, m30),
            FVector4(m01, m11, m21, m31),
            FVector4(m02, m12, m22, m32),
            FVector4(m03, m13, m23, m33)
        };
        return self * RotationMatrix;
    }

    constexpr auto TransformDirection(this FMatrix4x4 const& self, FVector3 const& Direction) -> FVector3 {
        return Direction * static_cast<FMatrix3x3>(self);
    }

    constexpr auto operator [](this FMatrix4x4 const& self, std::size_t const rows, std::size_t const cols) -> std::float_t {
        return self.Columns[cols][rows];
    }

    friend constexpr auto operator *(FMatrix4x4 const& first, FVector4 const& second) -> FVector4 {
        return first.Columns[0] * second[0]
             + first.Columns[1] * second[1]
             + first.Columns[2] * second[2]
             + first.Columns[3] * second[3];
    }

    friend constexpr auto operator *(FVector4 const& first, FMatrix4x4 const& second) -> FVector4 {
        return first[0] * FVector4(second[0, 0], second[0, 1], second[0, 2], second[0, 3])
             + first[1] * FVector4(second[1, 0], second[1, 1], second[1, 2], second[1, 3])
             + first[2] * FVector4(second[2, 0], second[2, 1], second[2, 2], second[2, 3])
             + first[3] * FVector4(second[3, 0], second[3, 1], second[3, 2], second[3, 3]);
    }

    friend constexpr auto operator +(FMatrix4x4 const& first, FMatrix4x4 const& second) -> FMatrix4x4 {
        return FMatrix4x4{
            first.Columns[0] + second.Columns[0],
            first.Columns[1] + second.Columns[1],
            first.Columns[2] + second.Columns[2],
            first.Columns[3] + second.Columns[3]
        };
    }

    friend constexpr auto operator -(FMatrix4x4 const& first, FMatrix4x4 const& second) -> FMatrix4x4 {
        return FMatrix4x4{
            first.Columns[0] - second.Columns[0],
            first.Columns[1] - second.Columns[1],
            first.Columns[2] - second.Columns[2],
            first.Columns[3] - second.Columns[3]
        };
    }

    friend constexpr auto operator *(FMatrix4x4 const& first, FMatrix4x4 const& second) -> FMatrix4x4 {
        return FMatrix4x4{
            first * second.Columns[0],
            first * second.Columns[1],
            first * second.Columns[2],
            first * second.Columns[3]
        };
    }
};