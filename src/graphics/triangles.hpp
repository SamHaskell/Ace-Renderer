#pragma once

#include "core/defines.hpp"
#include "maths/vector.hpp"
#include "graphics/color.hpp"

#define MAX_POLY_VERTICES 12

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
    
        Vec3 BarycentricWeights(Vec2 point);
        Vec3 InterpolatedUVW(Vec3 barycentricWeights);
    };

    struct Polygon {
        Vertex Vertices[MAX_POLY_VERTICES];
        i32 VertexCount;
    };
}