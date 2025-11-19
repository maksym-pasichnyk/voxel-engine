//
// Created by Maksym Pasichnyk on 26.07.2025.
//
#pragma once

#include <cmath>
#include <memory>
#include <compare>

struct FVector2I {
    std::int32_t x;
    std::int32_t y;

    constexpr FVector2I() : x(0), y(0) {}
    explicit constexpr FVector2I(std::int32_t const scalar) : x(scalar), y(scalar) {}
    explicit constexpr FVector2I(std::int32_t const x, std::int32_t const y) : x(x), y(y) {}

    constexpr auto operator[](this FVector2I& self, std::size_t const i) -> std::int32_t& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            default: std::unreachable();
        }
    }

    constexpr auto operator[](this FVector2I const& self, std::size_t const i) -> std::int32_t const& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            default: std::unreachable();
        }
    }

    friend constexpr auto operator<=>(FVector2I const&, FVector2I const&) -> std::strong_ordering = default;

    friend constexpr auto operator+(FVector2I const& first, FVector2I const& other) -> FVector2I {
        return FVector2I(first.x + other.x, first.y + other.y);
    }

    friend constexpr auto operator-(FVector2I const& first, FVector2I const& other) -> FVector2I {
        return FVector2I(first.x - other.x, first.y - other.y);
    }

    friend constexpr auto operator*(FVector2I const& first, FVector2I const& other) -> FVector2I {
        return FVector2I(first.x * other.x, first.y * other.y);
    }

    friend constexpr auto operator/(FVector2I const& first, FVector2I const& other) -> FVector2I {
        return FVector2I(first.x / other.x, first.y / other.y);
    }

    friend constexpr auto operator+(FVector2I const& first, std::int32_t const other) -> FVector2I {
        return FVector2I(first.x + other, first.y + other);
    }

    friend constexpr auto operator-(FVector2I const& first, std::int32_t const other) -> FVector2I {
        return FVector2I(first.x - other, first.y - other);
    }

    friend constexpr auto operator*(FVector2I const& first, std::int32_t const other) -> FVector2I {
        return FVector2I(first.x * other, first.y * other);
    }

    friend constexpr auto operator/(FVector2I const& first, std::int32_t const other) -> FVector2I {
        return FVector2I(first.x / other, first.y / other);
    }

    friend constexpr auto operator+(std::int32_t first, FVector2I const& other) -> FVector2I {
        return FVector2I(first + other.x, first + other.y);
    }

    friend constexpr auto operator-(std::int32_t first, FVector2I const& other) -> FVector2I {
        return FVector2I(first - other.x, first - other.y);
    }

    friend constexpr auto operator*(std::int32_t first, FVector2I const& other) -> FVector2I {
        return FVector2I(first * other.x, first * other.y);
    }

    friend constexpr auto operator/(std::int32_t first, FVector2I const& other) -> FVector2I {
        return FVector2I(first / other.x, first / other.y);
    }

    constexpr auto Dot(this FVector2I const& self, FVector2I const& other) -> std::int32_t {
        return self.x * other.x + self.y * self.y;
    }
    // constexpr auto Length(this FVector2I const& self) -> std::float_t {
    //     return std::sqrt(self.Dot(self));
    // }
    //
    // constexpr auto Normalized(this FVector2I const& self) -> FVector2I {
    //     return self / self.Length();
    // }
};
