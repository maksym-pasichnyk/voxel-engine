//
// Created by Maksym Pasichnyk on 18.06.2025.
//
#pragma once

#include <cmath>
#include <memory>
#include <compare>

struct FVector2D {
    std::double_t x;
    std::double_t y;

    constexpr FVector2D() : x(0), y(0) {}
    explicit constexpr FVector2D(std::double_t const scalar) : x(scalar), y(scalar) {}
    explicit constexpr FVector2D(std::double_t const x, std::double_t const y) : x(x), y(y) {}

    constexpr auto operator[](this FVector2D& self, std::size_t const i) -> std::double_t& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            default: std::unreachable();
        }
    }

    constexpr auto operator[](this FVector2D const& self, std::size_t const i) -> std::double_t const& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            default: std::unreachable();
        }
    }

    friend constexpr auto operator<=>(FVector2D const&, FVector2D const&) -> std::strong_ordering = default;

    friend constexpr auto operator+(FVector2D const& first, FVector2D const& other) -> FVector2D {
        return FVector2D(first.x + other.x, first.y + other.y);
    }

    friend constexpr auto operator-(FVector2D const& first, FVector2D const& other) -> FVector2D {
        return FVector2D(first.x - other.x, first.y - other.y);
    }

    friend constexpr auto operator*(FVector2D const& first, FVector2D const& other) -> FVector2D {
        return FVector2D(first.x * other.x, first.y * other.y);
    }

    friend constexpr auto operator/(FVector2D const& first, FVector2D const& other) -> FVector2D {
        return FVector2D(first.x / other.x, first.y / other.y);
    }

    friend constexpr auto operator+(FVector2D const& first, std::double_t const other) -> FVector2D {
        return FVector2D(first.x + other, first.y + other);
    }

    friend constexpr auto operator-(FVector2D const& first, std::double_t const other) -> FVector2D {
        return FVector2D(first.x - other, first.y - other);
    }

    friend constexpr auto operator*(FVector2D const& first, std::double_t const other) -> FVector2D {
        return FVector2D(first.x * other, first.y * other);
    }

    friend constexpr auto operator/(FVector2D const& first, std::double_t const other) -> FVector2D {
        return FVector2D(first.x / other, first.y / other);
    }

    friend constexpr auto operator+(std::double_t first, FVector2D const& other) -> FVector2D {
        return FVector2D(first + other.x, first + other.y);
    }

    friend constexpr auto operator-(std::double_t first, FVector2D const& other) -> FVector2D {
        return FVector2D(first - other.x, first - other.y);
    }

    friend constexpr auto operator*(std::double_t first, FVector2D const& other) -> FVector2D {
        return FVector2D(first * other.x, first * other.y);
    }

    friend constexpr auto operator/(std::double_t first, FVector2D const& other) -> FVector2D {
        return FVector2D(first / other.x, first / other.y);
    }

    constexpr auto Dot(this FVector2D const& self, FVector2D const& other) -> std::double_t {
        return self.x * other.x + self.y * self.y;
    }

    constexpr auto Length(this FVector2D const& self) -> std::double_t {
        return std::sqrt(self.Dot(self));
    }

    constexpr auto Normalized(this FVector2D const& self) -> FVector2D {
        return self / self.Length();
    }

    constexpr auto Ceil(this FVector2D const& self) -> FVector2D {
        return FVector2D(std::ceil(self.x), std::ceil(self.y));
    }

    constexpr auto Round(this FVector2D const& self) -> FVector2D {
        return FVector2D(std::round(self.x), std::round(self.y));
    }

    constexpr auto Floor(this FVector2D const& self) -> FVector2D {
        return FVector2D(std::floor(self.x), std::floor(self.y));
    }

    constexpr auto Sin(this FVector2D const& self) -> FVector2D {
        return FVector2D(std::sin(self.x), std::sin(self.y));
    }

    constexpr auto Cos(this FVector2D const& self) -> FVector2D {
        return FVector2D(std::cos(self.x), std::cos(self.y));
    }
};
