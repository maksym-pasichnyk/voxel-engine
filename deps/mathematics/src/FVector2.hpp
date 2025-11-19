//
// Created by Maksym Pasichnyk on 15.06.2025.
//
#pragma once

#include <cmath>
#include <memory>
#include <compare>

struct FVector2 {
    std::float_t x;
    std::float_t y;

    constexpr FVector2() : x(0), y(0) {}
    explicit constexpr FVector2(std::float_t const scalar) : x(scalar), y(scalar) {}
    explicit constexpr FVector2(std::float_t const x, std::float_t const y) : x(x), y(y) {}

    constexpr auto operator[](this FVector2& self, std::size_t const i) -> std::float_t& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            default: std::unreachable();
        }
    }

    constexpr auto operator[](this FVector2 const& self, std::size_t const i) -> std::float_t const& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            default: std::unreachable();
        }
    }

    friend constexpr auto operator<=>(FVector2 const&, FVector2 const&) -> std::strong_ordering = default;

    friend constexpr auto operator+(FVector2 const& first, FVector2 const& other) -> FVector2 {
        return FVector2(first.x + other.x, first.y + other.y);
    }

    friend constexpr auto operator-(FVector2 const& first, FVector2 const& other) -> FVector2 {
        return FVector2(first.x - other.x, first.y - other.y);
    }

    friend constexpr auto operator*(FVector2 const& first, FVector2 const& other) -> FVector2 {
        return FVector2(first.x * other.x, first.y * other.y);
    }

    friend constexpr auto operator/(FVector2 const& first, FVector2 const& other) -> FVector2 {
        return FVector2(first.x / other.x, first.y / other.y);
    }

    friend constexpr auto operator+(FVector2 const& first, std::float_t const other) -> FVector2 {
        return FVector2(first.x + other, first.y + other);
    }

    friend constexpr auto operator-(FVector2 const& first, std::float_t const other) -> FVector2 {
        return FVector2(first.x - other, first.y - other);
    }

    friend constexpr auto operator*(FVector2 const& first, std::float_t const other) -> FVector2 {
        return FVector2(first.x * other, first.y * other);
    }

    friend constexpr auto operator/(FVector2 const& first, std::float_t const other) -> FVector2 {
        return FVector2(first.x / other, first.y / other);
    }

    friend constexpr auto operator+(std::float_t first, FVector2 const& other) -> FVector2 {
        return FVector2(first + other.x, first + other.y);
    }

    friend constexpr auto operator-(std::float_t first, FVector2 const& other) -> FVector2 {
        return FVector2(first - other.x, first - other.y);
    }

    friend constexpr auto operator*(std::float_t first, FVector2 const& other) -> FVector2 {
        return FVector2(first * other.x, first * other.y);
    }

    friend constexpr auto operator/(std::float_t first, FVector2 const& other) -> FVector2 {
        return FVector2(first / other.x, first / other.y);
    }

    constexpr auto Dot(this FVector2 const& self, FVector2 const& other) -> std::float_t {
        return self.x * other.x + self.y * self.y;
    }

    constexpr auto Length(this FVector2 const& self) -> std::float_t {
        return std::sqrt(self.Dot(self));
    }

    constexpr auto Normalized(this FVector2 const& self) -> FVector2 {
        return self / self.Length();
    }

    constexpr auto Ceil(this FVector2 const& self) -> FVector2 {
        return FVector2(std::ceil(self.x), std::ceil(self.y));
    }

    constexpr auto Round(this FVector2 const& self) -> FVector2 {
        return FVector2(std::round(self.x), std::round(self.y));
    }

    constexpr auto Floor(this FVector2 const& self) -> FVector2 {
        return FVector2(std::floor(self.x), std::floor(self.y));
    }

    constexpr auto Sin(this FVector2 const& self) -> FVector2 {
        return FVector2(std::sin(self.x), std::sin(self.y));
    }

    constexpr auto Cos(this FVector2 const& self) -> FVector2 {
        return FVector2(std::cos(self.x), std::cos(self.y));
    }
};
