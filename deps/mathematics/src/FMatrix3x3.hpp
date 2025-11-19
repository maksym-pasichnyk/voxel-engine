//
// Created by Maksym Pasichnyk on 26.07.2025.
//
#pragma once

#include "FVector3.hpp"

struct FMatrix3x3 {
    FVector3 Columns[3] = {};

    static constexpr auto Identity() -> FMatrix3x3 {
        return FMatrix3x3{
           FVector3(1, 0, 0),
           FVector3(0, 1, 0),
           FVector3(0, 0, 1)
        };
    }

    constexpr auto Transpose(this FMatrix3x3 const& self) -> FMatrix3x3 {
        return FMatrix3x3{
            FVector3(self.Columns[0][0], self.Columns[1][0], self.Columns[2][0]),
            FVector3(self.Columns[0][1], self.Columns[1][1], self.Columns[2][1]),
            FVector3(self.Columns[0][2], self.Columns[1][2], self.Columns[2][2]),
        };
    }

    constexpr auto operator [](this FMatrix3x3 const& self, std::size_t const rows, std::size_t const cols) -> std::float_t {
        return self.Columns[cols][rows];
    }

    friend constexpr auto operator *(FMatrix3x3 const& first, FVector3 const& second) -> FVector3 {
        return first.Columns[0] * second[0]
             + first.Columns[1] * second[1]
             + first.Columns[2] * second[2];
    }

    friend constexpr auto operator *(FVector3 const& first, FMatrix3x3 const& second) -> FVector3 {
        return first[0] * FVector3(second[0, 0], second[0, 1], second[0, 2])
             + first[1] * FVector3(second[1, 0], second[1, 1], second[1, 2])
             + first[2] * FVector3(second[2, 0], second[2, 1], second[2, 2]);
    }

    friend constexpr auto operator +(FMatrix3x3 const& first, FMatrix3x3 const& second) -> FMatrix3x3 {
        return FMatrix3x3{
            first.Columns[0] + second.Columns[0],
            first.Columns[1] + second.Columns[1],
            first.Columns[2] + second.Columns[2],
        };
    }

    friend constexpr auto operator -(FMatrix3x3 const& first, FMatrix3x3 const& second) -> FMatrix3x3 {
        return FMatrix3x3{
            first.Columns[0] - second.Columns[0],
            first.Columns[1] - second.Columns[1],
            first.Columns[2] - second.Columns[2],
        };
    }

    friend constexpr auto operator *(FMatrix3x3 const& first, FMatrix3x3 const& second) -> FMatrix3x3 {
        return FMatrix3x3{
            first * second.Columns[0],
            first * second.Columns[1],
            first * second.Columns[2],
        };
    }
};