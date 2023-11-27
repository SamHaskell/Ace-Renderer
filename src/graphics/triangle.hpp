#pragma once

#include "core/defines.hpp"
#include "maths/vector.hpp"
#include "graphics/color.hpp"

namespace Ace {
    struct Vertex {
        Vec4 Position;
        Vec2 TexCoord;
    };

    struct Face {
        u32 a;
        u32 b;
        u32 c;
        u32 aUV;
        u32 bUV;
        u32 cUV;
    };

    struct Triangle {
        Vertex Vertices[3];
        f32 Depth;
        Color Color;
    };
}