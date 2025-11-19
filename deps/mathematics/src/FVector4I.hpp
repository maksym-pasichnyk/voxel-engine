//
// Created by Maksym Pasichnyk on 15.06.2025.
//
#pragma once

#include "FVector2I.hpp"
#include "FVector3I.hpp"

struct FVector4I {
    std::int32_t x;
    std::int32_t y;
    std::int32_t z;
    std::int32_t w;

    constexpr FVector4I() : x(0), y(0), z(0), w(0) {}
    explicit constexpr FVector4I(std::int32_t const scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    explicit constexpr FVector4I(std::int32_t const x, std::int32_t const y, std::int32_t const z, std::int32_t const w) : x(x), y(y), z(z), w(w) {}
    explicit constexpr FVector4I(FVector2I const xy, std::int32_t const z, std::int32_t const w) : x(xy.x), y(xy.y), z(z), w(w) {}
    explicit constexpr FVector4I(std::int32_t const x, FVector2I const yz, std::int32_t const w) : x(x), y(yz.x), z(yz.y), w(w) {}
    explicit constexpr FVector4I(std::int32_t const x, std::int32_t const y, FVector2I const zw) : x(x), y(y), z(zw.x), w(zw.y) {}
    explicit constexpr FVector4I(FVector2I const xy, FVector2I const zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
    explicit constexpr FVector4I(FVector3I const xyz, std::int32_t const w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
    explicit constexpr FVector4I(std::int32_t const x, FVector3I const yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
    
    constexpr operator FVector3I(this FVector4I const& This) {
        return FVector3I(This.x, This.y, This.z);
    }

    constexpr auto operator[](this FVector4I& self, std::size_t const i) -> std::int32_t& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            case 3: return self.w;
            default: std::unreachable();
        }
    }
    constexpr auto operator[](this FVector4I const& self, std::size_t const i) -> std::int32_t const& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            case 3: return self.w;
            default: std::unreachable();
        }
    }

    friend constexpr auto operator<=>(FVector4I const&, FVector4I const&) -> std::strong_ordering = default;

    friend constexpr auto operator+(FVector4I const& first, FVector4I const& other) -> FVector4I {
        return FVector4I(first.x + other.x, first.y + other.y, first.z + other.z, first.w + other.w);
    }

    friend constexpr auto operator-(FVector4I const& first, FVector4I const& other) -> FVector4I {
        return FVector4I(first.x - other.x, first.y - other.y, first.z - other.z, first.w - other.w);
    }

    friend constexpr auto operator*(FVector4I const& first, FVector4I const& other) -> FVector4I {
        return FVector4I(first.x * other.x, first.y * other.y, first.z * other.z, first.w * other.w);
    }

    friend constexpr auto operator/(FVector4I const& first, FVector4I const& other) -> FVector4I {
        return FVector4I(first.x / other.x, first.y / other.y, first.z / other.z, first.w / other.w);
    }

    friend constexpr auto operator+(FVector4I const& first, std::int32_t const other) -> FVector4I {
        return FVector4I(first.x + other, first.y + other, first.z + other, first.w + other);
    }

    friend constexpr auto operator-(FVector4I const& first, std::int32_t const other) -> FVector4I {
        return FVector4I(first.x - other, first.y - other, first.z - other, first.w - other);
    }

    friend constexpr auto operator*(FVector4I const& first, std::int32_t const other) -> FVector4I {
        return FVector4I(first.x * other, first.y * other, first.z * other, first.w * other);
    }

    friend constexpr auto operator/(FVector4I const& first, std::int32_t const other) -> FVector4I {
        return FVector4I(first.x / other, first.y / other, first.z / other, first.w / other);
    }

    friend constexpr auto operator+(std::int32_t first, FVector4I const& other) -> FVector4I {
        return FVector4I(first + other.x, first + other.y, first + other.z, first + other.w);
    }

    friend constexpr auto operator-(std::int32_t first, FVector4I const& other) -> FVector4I {
        return FVector4I(first - other.x, first - other.y, first - other.z, first - other.w);
    }

    friend constexpr auto operator*(std::int32_t first, FVector4I const& other) -> FVector4I {
        return FVector4I(first * other.x, first * other.y, first * other.z, first * other.w);
    }

    friend constexpr auto operator/(std::int32_t first, FVector4I const& other) -> FVector4I {
        return FVector4I(first / other.x, first / other.y, first / other.z, first / other.w);
    }

    friend constexpr auto operator+=(FVector4I& first, FVector4I const& other) -> FVector4I& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector4I& first, FVector4I const& other) -> FVector4I& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector4I& first, FVector4I const& other) -> FVector4I& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector4I& first, FVector4I const& other) -> FVector4I& {
        first = first / other;
        return first;
    }

    friend constexpr auto operator+=(FVector4I& first, std::int32_t const other) -> FVector4I& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector4I& first, std::int32_t const other) -> FVector4I& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector4I& first, std::int32_t const other) -> FVector4I& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector4I& first, std::int32_t const other) -> FVector4I& {
        first = first / other;
        return first;
    }

    // constexpr auto Dot(this FVector4I const& self, FVector4I const& other) -> std::int32_t {
    //     return self.x * other.x + self.y * self.y + self.z * other.z + self.w * other.w;
    // }
    //
    // constexpr auto Length(this FVector4I const& self) -> std::float_t {
    //     return std::sqrt(static_cast<std::float_t>(self.Dot(self)));
    // }
    // constexpr auto Normalized(this FVector4I const& self) -> FVector4I {
    //     return self / self.Length();
    // }
    //
    // constexpr auto Ceil(this FVector4I const& self) -> FVector4I {
    //     return FVector4I(std::ceil(self.x), std::ceil(self.y), std::ceil(self.z), std::ceil(self.w));
    // }
    //
    // constexpr auto Round(this FVector4I const& self) -> FVector4I {
    //     return FVector4I(std::round(self.x), std::round(self.y), std::round(self.z), std::round(self.w));
    // }
    //
    // constexpr auto Floor(this FVector4I const& self) -> FVector4I {
    //     return FVector4I(std::floor(self.x), std::floor(self.y), std::floor(self.z), std::floor(self.w));
    // }
    //
    // constexpr auto Sin(this FVector4I const& self) -> FVector4I {
    //     return FVector4I(std::sin(self.x), std::sin(self.y), std::sin(self.z), std::sin(self.w));
    // }
    //
    // constexpr auto Cos(this FVector4I const& self) -> FVector4I {
    //     return FVector4I(std::cos(self.x), std::cos(self.y), std::cos(self.z), std::cos(self.w));
    // }
};