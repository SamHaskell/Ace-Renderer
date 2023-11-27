#pragma once

#include "core/defines.hpp"
#include "maths/vector.hpp"
#include "graphics/color.hpp"

namespace Ace {
    struct Face {
        u32 a;
        u32 b;
        u32 c;
        u32 aUV;
        u32 bUV;
        u32 cUV;
    };

    struct Triangle {
        Vec2 Points[3];
        Vec2 TexCoords[3];
        f32 Depth;
        Color Color;
    };
}