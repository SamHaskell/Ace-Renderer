#include "graphics/camera.hpp"

namespace Ace {
    Mat4 Camera::GetViewMatrix(Vec3 up) {
        Vec3 forward = Mat4::Rotation(Rotation) * Vec3(0.0f, 0.0f, 1.0f);
        return Mat4::LookAt(Position, Position + forward, up);
    }
};