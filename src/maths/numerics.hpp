#pragma once

#include "core/defines.hpp"

namespace Ace {
    #define EvalPrintFloat(x) printf("%s = %f\n", #x, (f32)(x))

    inline f32 Clamp(f32 x, f32 min, f32 max) {
        return ( x < min ) ? x : (( x > max ) ? max : x);
    }

    inline f64 Clamp(f64 x, f64 min, f64 max) {
        return ( x < min ) ? x : (( x > max ) ? max : x);
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