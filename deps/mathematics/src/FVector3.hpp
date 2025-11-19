//
// Created by Maksym Pasichnyk on 15.06.2025.
//
#pragma once

#include "FVector2.hpp"

struct FVector3 {
    std::float_t x;
    std::float_t y;
    std::float_t z;

    constexpr FVector3() : x(0), y(0), z(0) {}
    explicit constexpr FVector3(std::float_t const scalar) : x(scalar), y(scalar), z(scalar) {}
    explicit constexpr FVector3(std::float_t const x, std::float_t const y, std::float_t const z) : x(x), y(y), z(z) {}
    explicit constexpr FVector3(FVector2 const xy, std::float_t const z) : x(xy.x), y(xy.y), z(z) {}
    explicit constexpr FVector3(std::float_t const x, FVector2 const yz) : x(x), y(yz.x), z(yz.y) {}

    constexpr auto operator[](this FVector3& self, std::size_t const i) -> std::float_t& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            default: std::unreachable();
        }
    }
    constexpr auto operator[](this FVector3 const& self, std::size_t const i) -> std::float_t const& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            default: std::unreachable();
        }
    }

    friend constexpr auto operator<=>(FVector3 const&, FVector3 const&) -> std::strong_ordering = default;

    friend constexpr auto operator+(FVector3 const& first, FVector3 const& other) -> FVector3 {
        return FVector3(first.x + other.x, first.y + other.y, first.z + other.z);
    }

    friend constexpr auto operator-(FVector3 const& first, FVector3 const& other) -> FVector3 {
        return FVector3(first.x - other.x, first.y - other.y, first.z - other.z);
    }

    friend constexpr auto operator*(FVector3 const& first, FVector3 const& other) -> FVector3 {
        return FVector3(first.x * other.x, first.y * other.y, first.z * other.z);
    }

    friend constexpr auto operator/(FVector3 const& first, FVector3 const& other) -> FVector3 {
        return FVector3(first.x / other.x, first.y / other.y, first.z / other.z);
    }

    friend constexpr auto operator+(FVector3 const& first, std::float_t other) -> FVector3 {
        return FVector3(first.x + other, first.y + other, first.z + other);
    }

    friend constexpr auto operator-(FVector3 const& first, std::float_t other) -> FVector3 {
        return FVector3(first.x - other, first.y - other, first.z - other);
    }

    friend constexpr auto operator*(FVector3 const& first, std::float_t other) -> FVector3 {
        return FVector3(first.x * other, first.y * other, first.z * other);
    }

    friend constexpr auto operator/(FVector3 const& first, std::float_t other) -> FVector3 {
        return FVector3(first.x / other, first.y / other, first.z / other);
    }

    friend constexpr auto operator+(std::float_t first, FVector3 const& other) -> FVector3 {
        return FVector3(first + other.x, first + other.y, first + other.z);
    }

    friend constexpr auto operator-(std::float_t first, FVector3 const& other) -> FVector3 {
        return FVector3(first - other.x, first - other.y, first - other.z);
    }

    friend constexpr auto operator*(std::float_t first, FVector3 const& other) -> FVector3 {
        return FVector3(first * other.x, first * other.y, first * other.z);
    }

    friend constexpr auto operator/(std::float_t first, FVector3 const& other) -> FVector3 {
        return FVector3(first / other.x, first / other.y, first / other.z);
    }

    friend constexpr auto operator+=(FVector3& first, FVector3 const& other) -> FVector3& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector3& first, FVector3 const& other) -> FVector3& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector3& first, FVector3 const& other) -> FVector3& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector3& first, FVector3 const& other) -> FVector3& {
        first = first / other;
        return first;
    }

    friend constexpr auto operator+=(FVector3& first, std::float_t const other) -> FVector3& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector3& first, std::float_t const other) -> FVector3& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector3& first, std::float_t const other) -> FVector3& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector3& first, std::float_t const other) -> FVector3& {
        first = first / other;
        return first;
    }

    constexpr auto Dot(this FVector3 const& self, FVector3 const& other) -> std::float_t {
        return self.x * other.x + self.y * self.y + self.z * other.z;
    }

    constexpr auto Length(this FVector3 const& self) -> std::float_t {
        return std::sqrt(self.Dot(self));
    }

    constexpr auto Normalized(this FVector3 const& self) -> FVector3 {
        return self / self.Length();
    }

    constexpr auto Cross(this FVector3 const& self, FVector3 const& other) -> FVector3 {
        auto const A = FVector3(self.y * other.z, self.z * other.x, self.x * other.y);
        auto const B = FVector3(self.z * other.y, self.x * other.z, self.y * other.x);
        return A - B;
    }

    constexpr auto Ceil(this FVector3 const& self) -> FVector3 {
        return FVector3(std::ceil(self.x), std::ceil(self.y), std::ceil(self.z));
    }

    constexpr auto Round(this FVector3 const& self) -> FVector3 {
        return FVector3(std::round(self.x), std::round(self.y), std::round(self.z));
    }

    constexpr auto Floor(this FVector3 const& self) -> FVector3 {
        return FVector3(std::floor(self.x), std::floor(self.y), std::floor(self.z));
    }

    constexpr auto Sin(this FVector3 const& self) -> FVector3 {
        return FVector3(std::sin(self.x), std::sin(self.y), std::sin(self.z));
    }

    constexpr auto Cos(this FVector3 const& self) -> FVector3 {
        return FVector3(std::cos(self.x), std::cos(self.y), std::cos(self.z));
    }
};
