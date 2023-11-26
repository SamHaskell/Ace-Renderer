#pragma once

#include "core/defines.hpp"
#include "maths/vector.hpp"

namespace Ace {
    struct Face {
        u32 a;
        u32 b;
        u32 c;
    };

    struct Triangle {
        Vec2 points[3];
        f32 depth;
    };
}