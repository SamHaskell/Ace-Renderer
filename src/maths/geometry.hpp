#pragma once

#include "core/defines.hpp"
#include "maths/vector.hpp"

namespace Ace {
    struct Rect {
        f32 x;
        f32 y;
        f32 w;
        f32 h;
    };

    struct Plane {
        Vec3 Point;
        Vec3 Normal;
    };
};