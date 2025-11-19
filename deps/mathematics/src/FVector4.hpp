//
// Created by Maksym Pasichnyk on 15.06.2025.
//
#pragma once

#include "FVector2.hpp"
#include "FVector3.hpp"

struct FVector4 {
    std::float_t x;
    std::float_t y;
    std::float_t z;
    std::float_t w;

    constexpr FVector4() : x(0), y(0), z(0), w(0) {}
    explicit constexpr FVector4(std::float_t const scalar) : x(scalar), y(scalar), z(scalar), w(scalar) {}
    explicit constexpr FVector4(std::float_t const x, std::float_t const y, std::float_t const z, std::float_t const w) : x(x), y(y), z(z), w(w) {}
    explicit constexpr FVector4(FVector2 const xy, std::float_t const z, std::float_t const w) : x(xy.x), y(xy.y), z(z), w(w) {}
    explicit constexpr FVector4(std::float_t const x, FVector2 const yz, std::float_t const w) : x(x), y(yz.x), z(yz.y), w(w) {}
    explicit constexpr FVector4(std::float_t const x, std::float_t const y, FVector2 const zw) : x(x), y(y), z(zw.x), w(zw.y) {}
    explicit constexpr FVector4(FVector2 const xy, FVector2 const zw) : x(xy.x), y(xy.y), z(zw.x), w(zw.y) {}
    explicit constexpr FVector4(FVector3 const xyz, std::float_t const w) : x(xyz.x), y(xyz.y), z(xyz.z), w(w) {}
    explicit constexpr FVector4(std::float_t const x, FVector3 const yzw) : x(x), y(yzw.x), z(yzw.y), w(yzw.z) {}
    
    constexpr operator FVector3(this FVector4 const& This) {
        return FVector3(This.x, This.y, This.z);
    }

    constexpr auto operator[](this FVector4& self, std::size_t const i) -> std::float_t& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            case 3: return self.w;
            default: std::unreachable();
        }
    }
    constexpr auto operator[](this FVector4 const& self, std::size_t const i) -> std::float_t const& {
        switch (i) {
            case 0: return self.x;
            case 1: return self.y;
            case 2: return self.z;
            case 3: return self.w;
            default: std::unreachable();
        }
    }


    friend constexpr auto operator<=>(FVector4 const&, FVector4 const&) -> std::strong_ordering = default;

    friend constexpr auto operator+(FVector4 const& first, FVector4 const& other) -> FVector4 {
        return FVector4(first.x + other.x, first.y + other.y, first.z + other.z, first.w + other.w);
    }

    friend constexpr auto operator-(FVector4 const& first, FVector4 const& other) -> FVector4 {
        return FVector4(first.x - other.x, first.y - other.y, first.z - other.z, first.w - other.w);
    }

    friend constexpr auto operator*(FVector4 const& first, FVector4 const& other) -> FVector4 {
        return FVector4(first.x * other.x, first.y * other.y, first.z * other.z, first.w * other.w);
    }

    friend constexpr auto operator/(FVector4 const& first, FVector4 const& other) -> FVector4 {
        return FVector4(first.x / other.x, first.y / other.y, first.z / other.z, first.w / other.w);
    }

    friend constexpr auto operator+(FVector4 const& first, std::float_t const other) -> FVector4 {
        return FVector4(first.x + other, first.y + other, first.z + other, first.w + other);
    }

    friend constexpr auto operator-(FVector4 const& first, std::float_t const other) -> FVector4 {
        return FVector4(first.x - other, first.y - other, first.z - other, first.w - other);
    }

    friend constexpr auto operator*(FVector4 const& first, std::float_t const other) -> FVector4 {
        return FVector4(first.x * other, first.y * other, first.z * other, first.w * other);
    }

    friend constexpr auto operator/(FVector4 const& first, std::float_t const other) -> FVector4 {
        return FVector4(first.x / other, first.y / other, first.z / other, first.w / other);
    }

    friend constexpr auto operator+(std::float_t first, FVector4 const& other) -> FVector4 {
        return FVector4(first + other.x, first + other.y, first + other.z, first + other.w);
    }

    friend constexpr auto operator-(std::float_t first, FVector4 const& other) -> FVector4 {
        return FVector4(first - other.x, first - other.y, first - other.z, first - other.w);
    }

    friend constexpr auto operator*(std::float_t first, FVector4 const& other) -> FVector4 {
        return FVector4(first * other.x, first * other.y, first * other.z, first * other.w);
    }

    friend constexpr auto operator/(std::float_t first, FVector4 const& other) -> FVector4 {
        return FVector4(first / other.x, first / other.y, first / other.z, first / other.w);
    }

    friend constexpr auto operator+=(FVector4& first, FVector4 const& other) -> FVector4& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector4& first, FVector4 const& other) -> FVector4& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector4& first, FVector4 const& other) -> FVector4& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector4& first, FVector4 const& other) -> FVector4& {
        first = first / other;
        return first;
    }

    friend constexpr auto operator+=(FVector4& first, std::float_t const other) -> FVector4& {
        first = first + other;
        return first;
    }

    friend constexpr auto operator-=(FVector4& first, std::float_t const other) -> FVector4& {
        first = first - other;
        return first;
    }

    friend constexpr auto operator*=(FVector4& first, std::float_t const other) -> FVector4& {
        first = first * other;
        return first;
    }

    friend constexpr auto operator/=(FVector4& first, std::float_t const other) -> FVector4& {
        first = first / other;
        return first;
    }

    constexpr auto Dot(this FVector4 const& self, FVector4 const& other) -> std::float_t {
        return self.x * other.x + self.y * self.y + self.z * other.z + self.w * other.w;
    }

    constexpr auto Length(this FVector4 const& self) -> std::float_t {
        return std::sqrt(self.Dot(self));
    }

    constexpr auto Normalized(this FVector4 const& self) -> FVector4 {
        return self / self.Length();
    }

    constexpr auto Ceil(this FVector4 const& self) -> FVector4 {
        return FVector4(std::ceil(self.x), std::ceil(self.y), std::ceil(self.z), std::ceil(self.w));
    }

    constexpr auto Round(this FVector4 const& self) -> FVector4 {
        return FVector4(std::round(self.x), std::round(self.y), std::round(self.z), std::round(self.w));
    }

    constexpr auto Floor(this FVector4 const& self) -> FVector4 {
        return FVector4(std::floor(self.x), std::floor(self.y), std::floor(self.z), std::floor(self.w));
    }

    constexpr auto Sin(this FVector4 const& self) -> FVector4 {
        return FVector4(std::sin(self.x), std::sin(self.y), std::sin(self.z), std::sin(self.w));
    }

    constexpr auto Cos(this FVector4 const& self) -> FVector4 {
        return FVector4(std::cos(self.x), std::cos(self.y), std::cos(self.z), std::cos(self.w));
    }
};