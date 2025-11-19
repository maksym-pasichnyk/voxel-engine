//
// Created by Maksym Pasichnyk on 15.06.2025.
//
#pragma once

#include "FVector2D.hpp"

struct FVector3D {
    std::double_t x;
    std::double_t y;
    std::double_t z;

    constexpr FVector3D() : x(0), y(0), z(0) {}
    explicit constexpr FVector3D(std::double_t const scalar) : x(scalar), y(scalar), z(scalar) {}
    explicit constexpr FVector3D(std::double_t const x, std::double_t const y, std::double_t const z) : x(x), y(y), z(z) {}
    explicit constexpr FVector3D(FVector2D const xy, std::double_t const z) : x(xy.x), y(xy.y), z(z) {}
    explicit constexpr FVector3D(std::double_t const x, FVector2D const yz) : x(x), y(yz.x), z(yz.y) {}

    constexpr auto operator[](this FVector3D& self, std::size_t const i) -> std::double_t& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            default: std::unreachable();
        }
    }
    constexpr auto operator[](this FVector3D const& self, std::size_t const i) -> std::double_t const& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            default: std::unreachable();
        }
    }

    friend constexpr auto operator<=>(FVector3D const&, FVector3D const&) -> std::strong_ordering = default;

    friend constexpr auto operator+(FVector3D const& first, FVector3D const& other) -> FVector3D {
        return FVector3D(first.x + other.x, first.y + other.y, first.z + other.z);
    }

    friend constexpr auto operator-(FVector3D const& first, FVector3D const& other) -> FVector3D {
        return FVector3D(first.x - other.x, first.y - other.y, first.z - other.z);
    }

    friend constexpr auto operator*(FVector3D const& first, FVector3D const& other) -> FVector3D {
        return FVector3D(first.x * other.x, first.y * other.y, first.z * other.z);
    }

    friend constexpr auto operator/(FVector3D const& first, FVector3D const& other) -> FVector3D {
        return FVector3D(first.x / other.x, first.y / other.y, first.z / other.z);
    }

    friend constexpr auto operator+(FVector3D const& first, std::double_t other) -> FVector3D {
        return FVector3D(first.x + other, first.y + other, first.z + other);
    }

    friend constexpr auto operator-(FVector3D const& first, std::double_t other) -> FVector3D {
        return FVector3D(first.x - other, first.y - other, first.z - other);
    }

    friend constexpr auto operator*(FVector3D const& first, std::double_t other) -> FVector3D {
        return FVector3D(first.x * other, first.y * other, first.z * other);
    }

    friend constexpr auto operator/(FVector3D const& first, std::double_t other) -> FVector3D {
        return FVector3D(first.x / other, first.y / other, first.z / other);
    }

    friend constexpr auto operator+(std::double_t first, FVector3D const& other) -> FVector3D {
        return FVector3D(first + other.x, first + other.y, first + other.z);
    }

    friend constexpr auto operator-(std::double_t first, FVector3D const& other) -> FVector3D {
        return FVector3D(first - other.x, first - other.y, first - other.z);
    }

    friend constexpr auto operator*(std::double_t first, FVector3D const& other) -> FVector3D {
        return FVector3D(first * other.x, first * other.y, first * other.z);
    }

    friend constexpr auto operator/(std::double_t first, FVector3D const& other) -> FVector3D {
        return FVector3D(first / other.x, first / other.y, first / other.z);
    }

    friend constexpr auto operator+=(FVector3D& first, FVector3D const& other) -> FVector3D& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector3D& first, FVector3D const& other) -> FVector3D& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector3D& first, FVector3D const& other) -> FVector3D& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector3D& first, FVector3D const& other) -> FVector3D& {
        first = first / other;
        return first;
    }

    friend constexpr auto operator+=(FVector3D& first, std::double_t const other) -> FVector3D& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector3D& first, std::double_t const other) -> FVector3D& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector3D& first, std::double_t const other) -> FVector3D& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector3D& first, std::double_t const other) -> FVector3D& {
        first = first / other;
        return first;
    }

    constexpr auto Dot(this FVector3D const& self, FVector3D const& other) -> std::double_t {
        return self.x * other.x + self.y * self.y + self.z * other.z;
    }

    constexpr auto Length(this FVector3D const& self) -> std::double_t {
        return std::sqrt(self.Dot(self));
    }

    constexpr auto Normalized(this FVector3D const& self) -> FVector3D {
        return self / self.Length();
    }

    constexpr auto Cross(this FVector3D const& self, FVector3D const& other) -> FVector3D {
        auto const A = FVector3D(self.y * other.z, self.z * other.x, self.x * other.y);
        auto const B = FVector3D(self.z * other.y, self.x * other.z, self.y * other.x);
        return A - B;
    }

    constexpr auto Ceil(this FVector3D const& self) -> FVector3D {
        return FVector3D(std::ceil(self.x), std::ceil(self.y), std::ceil(self.z));
    }

    constexpr auto Round(this FVector3D const& self) -> FVector3D {
        return FVector3D(std::round(self.x), std::round(self.y), std::round(self.z));
    }

    constexpr auto Floor(this FVector3D const& self) -> FVector3D {
        return FVector3D(std::floor(self.x), std::floor(self.y), std::floor(self.z));
    }

    constexpr auto Sin(this FVector3D const& self) -> FVector3D {
        return FVector3D(std::sin(self.x), std::sin(self.y), std::sin(self.z));
    }

    constexpr auto Cos(this FVector3D const& self) -> FVector3D {
        return FVector3D(std::cos(self.x), std::cos(self.y), std::cos(self.z));
    }
};
