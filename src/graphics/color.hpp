#pragma once

#include "core/defines.hpp"

namespace Ace {
    struct Color {
        // Color() = default;
        // Color(f32 r, f32 g, f32 b, f32 a) : r(r), g(g), b(b), a(a) {}

        f32 r = 1.0f;
        f32 g = 1.0f;
        f32 b = 1.0f;
        f32 a = 1.0f;

        u32 U32_ARGB();

        inline static Color White() { return {1.0f, 1.0f, 1.0f, 1.0f}; }
        inline static Color Black() { return {0.0f, 0.0f, 0.0f, 1.0f}; }
        inline static Color Red() { return {1.0f, 0.0f, 0.0f, 1.0f}; }
        inline static Color Green() { return {0.0f, 1.0f, 0.0f, 1.0f}; }
        inline static Color Blue() { return {0.0f, 0.0f, 1.0f, 1.0f}; }
    };

}