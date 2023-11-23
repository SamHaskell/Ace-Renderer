#pragma once

#include "core/defines.hpp"

namespace Ace {
    constexpr f64 PI = 3.14159265358979323846264;
    constexpr f64 TAU = 2 * PI;
    constexpr f64 EULER = 2.71828182845904523536028;

    constexpr f32 DEG2RAD = (f32)PI / 180.0f;
    constexpr f32 RAD2DEG = 180.0f / (f32)PI;

    #define EvalPrintFloat(x) printf("%s = %f\n", #x, (f32)(x))

    inline f32 Clamp(f32 x, f32 min, f32 max) {
        return ( x < min ) ? x : ( x > max ) ? max : x;
    }

    inline f64 Clamp(f64 x, f64 min, f64 max) {
        return ( x < min ) ? x : ( x > max ) ? max : x;
    }

    inline f32 Lerp(f32 a, f32 b, f32 t) {
        return a + ( b - a ) * t;
    }

    inline f32 LerpClamped(f32 a, f32 b, f32 t) {
        return a + ( b - a ) * Clamp(t, 0.0f, 1.0f);
    }

    inline f32 Unlerp(f32 a, f32 b, f32 y) {
        return ( y - a ) / ( b - a );
    }
}