//
// Created by Maksym Pasichnyk on 15.06.2025.
//
#pragma once

#include "FVector2I.hpp"

struct FVector3I {
    std::int32_t x;
    std::int32_t y;
    std::int32_t z;

    constexpr FVector3I() : x(0), y(0), z(0) {}
    explicit constexpr FVector3I(std::int32_t const scalar) : x(scalar), y(scalar), z(scalar) {}
    explicit constexpr FVector3I(std::int32_t const x, std::int32_t const y, std::int32_t const z) : x(x), y(y), z(z) {}
    explicit constexpr FVector3I(FVector2I const xy, std::int32_t const z) : x(xy.x), y(xy.y), z(z) {}
    explicit constexpr FVector3I(std::int32_t const x, FVector2I const yz) : x(x), y(yz.x), z(yz.y) {}

    constexpr auto operator[](this FVector3I& self, std::size_t const i) -> std::int32_t& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            default: std::unreachable();
        }
    }
    constexpr auto operator[](this FVector3I const& self, std::size_t const i) -> std::int32_t const& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            default: std::unreachable();
        }
    }

    friend constexpr auto operator<=>(FVector3I const&, FVector3I const&) -> std::strong_ordering = default;

    friend constexpr auto operator-(FVector3I const& self) -> FVector3I {
        return FVector3I(-self.x, -self.y, -self.z);
    }

    friend constexpr auto operator+(FVector3I const& first, FVector3I const& other) -> FVector3I {
        return FVector3I(first.x + other.x, first.y + other.y, first.z + other.z);
    }

    friend constexpr auto operator-(FVector3I const& first, FVector3I const& other) -> FVector3I {
        return FVector3I(first.x - other.x, first.y - other.y, first.z - other.z);
    }

    friend constexpr auto operator*(FVector3I const& first, FVector3I const& other) -> FVector3I {
        return FVector3I(first.x * other.x, first.y * other.y, first.z * other.z);
    }

    friend constexpr auto operator/(FVector3I const& first, FVector3I const& other) -> FVector3I {
        return FVector3I(first.x / other.x, first.y / other.y, first.z / other.z);
    }

    friend constexpr auto operator+(FVector3I const& first, std::int32_t other) -> FVector3I {
        return FVector3I(first.x + other, first.y + other, first.z + other);
    }

    friend constexpr auto operator-(FVector3I const& first, std::int32_t other) -> FVector3I {
        return FVector3I(first.x - other, first.y - other, first.z - other);
    }

    friend constexpr auto operator*(FVector3I const& first, std::int32_t other) -> FVector3I {
        return FVector3I(first.x * other, first.y * other, first.z * other);
    }

    friend constexpr auto operator/(FVector3I const& first, std::int32_t other) -> FVector3I {
        return FVector3I(first.x / other, first.y / other, first.z / other);
    }

    friend constexpr auto operator+(std::int32_t first, FVector3I const& other) -> FVector3I {
        return FVector3I(first + other.x, first + other.y, first + other.z);
    }

    friend constexpr auto operator-(std::int32_t first, FVector3I const& other) -> FVector3I {
        return FVector3I(first - other.x, first - other.y, first - other.z);
    }

    friend constexpr auto operator*(std::int32_t first, FVector3I const& other) -> FVector3I {
        return FVector3I(first * other.x, first * other.y, first * other.z);
    }

    friend constexpr auto operator/(std::int32_t first, FVector3I const& other) -> FVector3I {
        return FVector3I(first / other.x, first / other.y, first / other.z);
    }

    friend constexpr auto operator+=(FVector3I& first, FVector3I const& other) -> FVector3I& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector3I& first, FVector3I const& other) -> FVector3I& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector3I& first, FVector3I const& other) -> FVector3I& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector3I& first, FVector3I const& other) -> FVector3I& {
        first = first / other;
        return first;
    }

    friend constexpr auto operator+=(FVector3I& first, std::int32_t const other) -> FVector3I& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector3I& first, std::int32_t const other) -> FVector3I& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector3I& first, std::int32_t const other) -> FVector3I& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector3I& first, std::int32_t const other) -> FVector3I& {
        first = first / other;
        return first;
    }

    constexpr auto Dot(this FVector3I const& self, FVector3I const& other) -> std::int32_t {
        return self.x * other.x + self.y * self.y + self.z * other.z;
    }

    constexpr auto Length(this FVector3I const& self) -> std::float_t {
        return std::sqrt(static_cast<std::float_t>(self.Dot(self)));
    }

    // constexpr auto Normalized(this FVector3I const& self) -> FVector3I {
    //     return self / self.Length();
    // }

    constexpr auto Cross(this FVector3I const& self, FVector3I const& other) -> FVector3I {
        auto const A = FVector3I(self.y * other.z, self.z * other.x, self.x * other.y);
        auto const B = FVector3I(self.z * other.y, self.x * other.z, self.y * other.x);
        return A - B;
    }

    // constexpr auto Ceil(this FVector3I const& self) -> FVector3I {
    //     return FVector3I(std::ceil(self.x), std::ceil(self.y), std::ceil(self.z));
    // }
    //
    // constexpr auto Round(this FVector3I const& self) -> FVector3I {
    //     return FVector3I(std::round(self.x), std::round(self.y), std::round(self.z));
    // }
    //
    // constexpr auto Floor(this FVector3I const& self) -> FVector3I {
    //     return FVector3I(std::floor(self.x), std::floor(self.y), std::floor(self.z));
    // }
    //
    // constexpr auto Sin(this FVector3I const& self) -> FVector3I {
    //     return FVector3I(std::sin(self.x), std::sin(self.y), std::sin(self.z));
    // }
    //
    // constexpr auto Cos(this FVector3I const& self) -> FVector3I {
    //     return FVector3I(std::cos(self.x), std::cos(self.y), std::cos(self.z));
    // }
};
