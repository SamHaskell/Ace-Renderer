#pragma once

#include "core/defines.hpp"
#include "maths/maths.hpp"

namespace Ace {
    struct Camera {
        Vec3 Rotation = { 0.0f, 0.0f, 0.0f };
        Vec3 Position = { 0.0f, 0.0f, -5.0f };
        f32 FovY = 60.0f;
        f32 ZNear = 1.0f;
        f32 ZFar = 100.0f;

        Mat4 GetViewMatrix(Vec3 up = {0.0f, 1.0f, 0.0f});
    };
};