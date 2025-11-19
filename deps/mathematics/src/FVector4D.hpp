//
// Created by Maksym Pasichnyk on 15.06.2025.
//
#pragma once

#include "FVector2D.hpp"
#include "FVector3D.hpp"

struct FVector4D {
    std::double_t x;
    std::double_t y;
    std::double_t z;
    std::double_t w;

    constexpr FVector4D() : x(0), y(0), z(0), w(0) {}
    explicit constexpr FVector4D(std::double_t const scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    explicit constexpr FVector4D(std::double_t const x, std::double_t const y, std::double_t const z, std::double_t const w) : x(x), y(y), z(z), w(w) {}
    explicit constexpr FVector4D(FVector2D const xy, std::double_t const z, std::double_t const w) : x(xy.x), y(xy.y), z(z), w(w) {}
    explicit constexpr FVector4D(std::double_t const x, FVector2D const yz, std::double_t const w) : x(x), y(yz.x), z(yz.y), w(w) {}
    explicit constexpr FVector4D(std::double_t const x, std::double_t const y, FVector2D const zw) : x(x), y(y), z(zw.x), w(zw.y) {}
    explicit constexpr FVector4D(FVector2D const xy, FVector2D const zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
    explicit constexpr FVector4D(FVector3D const xyz, std::double_t const w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
    explicit constexpr FVector4D(std::double_t const x, FVector3D const yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}

    constexpr auto operator[](this FVector4D& self, std::size_t const i) -> std::double_t& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            case 3: return self.w;
            default: std::unreachable();
        }
    }
    constexpr auto operator[](this FVector4D const& self, std::size_t const i) -> std::double_t const& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            case 3: return self.w;
            default: std::unreachable();
        }
    }

    friend constexpr auto operator<=>(FVector4D const&, FVector4D const&) -> std::strong_ordering = default;

    friend constexpr auto operator+(FVector4D const& first, FVector4D const& other) -> FVector4D {
        return FVector4D(first.x + other.x, first.y + other.y, first.z + other.z, first.w + other.w);
    }

    friend constexpr auto operator-(FVector4D const& first, FVector4D const& other) -> FVector4D {
        return FVector4D(first.x - other.x, first.y - other.y, first.z - other.z, first.w - other.w);
    }

    friend constexpr auto operator*(FVector4D const& first, FVector4D const& other) -> FVector4D {
        return FVector4D(first.x * other.x, first.y * other.y, first.z * other.z, first.w * other.w);
    }

    friend constexpr auto operator/(FVector4D const& first, FVector4D const& other) -> FVector4D {
        return FVector4D(first.x / other.x, first.y / other.y, first.z / other.z, first.w / other.w);
    }

    friend constexpr auto operator+(FVector4D const& first, std::double_t other) -> FVector4D {
        return FVector4D(first.x + other, first.y + other, first.z + other, first.w + other);
    }

    friend constexpr auto operator-(FVector4D const& first, std::double_t other) -> FVector4D {
        return FVector4D(first.x - other, first.y - other, first.z - other, first.w - other);
    }

    friend constexpr auto operator*(FVector4D const& first, std::double_t other) -> FVector4D {
        return FVector4D(first.x * other, first.y * other, first.z * other, first.w * other);
    }

    friend constexpr auto operator/(FVector4D const& first, std::double_t other) -> FVector4D {
        return FVector4D(first.x / other, first.y / other, first.z / other, first.w / other);
    }

    friend constexpr auto operator+(std::double_t first, FVector4D const& other) -> FVector4D {
        return FVector4D(first + other.x, first + other.y, first + other.z, first + other.w);
    }

    friend constexpr auto operator-(std::double_t first, FVector4D const& other) -> FVector4D {
        return FVector4D(first - other.x, first - other.y, first - other.z, first - other.w);
    }

    friend constexpr auto operator*(std::double_t first, FVector4D const& other) -> FVector4D {
        return FVector4D(first * other.x, first * other.y, first * other.z, first * other.w);
    }

    friend constexpr auto operator/(std::double_t first, FVector4D const& other) -> FVector4D {
        return FVector4D(first / other.x, first / other.y, first / other.z, first / other.w);
    }

    friend constexpr auto operator+=(FVector4D& first, FVector4D const& other) -> FVector4D& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector4D& first, FVector4D const& other) -> FVector4D& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector4D& first, FVector4D const& other) -> FVector4D& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector4D& first, FVector4D const& other) -> FVector4D& {
        first = first / other;
        return first;
    }

    friend constexpr auto operator+=(FVector4D& first, std::double_t const other) -> FVector4D& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector4D& first, std::double_t const other) -> FVector4D& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector4D& first, std::double_t const other) -> FVector4D& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector4D& first, std::double_t const other) -> FVector4D& {
        first = first / other;
        return first;
    }

    constexpr auto Dot(this FVector4D const& self, FVector4D const& other) -> std::double_t {
        return self.x * other.x + self.y * self.y + self.z * other.z + self.w * other.w;
    }

    constexpr auto Length(this FVector4D const& self) -> std::double_t {
        return std::sqrt(self.Dot(self));
    }

    constexpr auto Normalized(this FVector4D const& self) -> FVector4D {
        return self / self.Length();
    }

    constexpr auto Ceil(this FVector4D const& self) -> FVector4D {
        return FVector4D(std::ceil(self.x), std::ceil(self.y), std::ceil(self.z), std::ceil(self.w));
    }

    constexpr auto Round(this FVector4D const& self) -> FVector4D {
        return FVector4D(std::round(self.x), std::round(self.y), std::round(self.z), std::round(self.w));
    }

    constexpr auto Floor(this FVector4D const& self) -> FVector4D {
        return FVector4D(std::floor(self.x), std::floor(self.y), std::floor(self.z), std::floor(self.w));
    }
    
    constexpr auto Sin(this FVector4D const& self) -> FVector4D {
        return FVector4D(std::sin(self.x), std::sin(self.y), std::sin(self.z), std::sin(self.w));
    }

    constexpr auto Cos(this FVector4D const& self) -> FVector4D {
        return FVector4D(std::cos(self.x), std::cos(self.y), std::cos(self.z), std::cos(self.w));
    }
};