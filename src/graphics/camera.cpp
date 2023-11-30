#include "graphics/camera.hpp"

namespace Ace {
    Mat4 Camera::GetViewMatrix(Vec3 up) {
        Vec3 forward = Mat4::Rotation(Rotation) * Vec3(0.0f, 0.0f, 1.0f);
        return Mat4::LookAt(Position, Position + forward, up);
    }

    Plane Camera::GetFrustumPlane(FrustumPlaneType frustumPlaneType, f32 aspectRatio) {
        Plane plane;

        f32 fovX = atan(tan(DEG2RAD * FovY / 2.0f) * aspectRatio) * 2.0f;

        f32 cosHalfFOVY = cos(DEG2RAD * FovY / 2.0f);
        f32 sinHalfFOVY = sin(DEG2RAD * FovY / 2.0f);

        f32 cosHalfFOVX = cos(fovX / 2.0f);
        f32 sinHalfFOVX = sin(fovX / 2.0f);

        switch (frustumPlaneType) {
            case FRUSTUM_LEFT:
            {
                plane.Point = {0.0f, 0.0f, 0.0f};
                plane.Normal = {cosHalfFOVX, 0.0f, sinHalfFOVX};
                break;
            }
            case FRUSTUM_RIGHT:
            {
                plane.Point = {0.0f, 0.0f, 0.0f};
                plane.Normal = {-cosHalfFOVX, 0.0f, sinHalfFOVX};
                break;
            }
            case FRUSTUM_TOP:
            {
                plane.Point = {0.0f, 0.0f, 0.0f};
                plane.Normal = {0.0f, -cosHalfFOVY, sinHalfFOVY};
                break;
            }
            case FRUSTUM_BOTTOM:
            {
                plane.Point = {0.0f, 0.0f, 0.0f};
                plane.Normal = {0.0f, cosHalfFOVY, sinHalfFOVY};
                break;
            }
            case FRUSTUM_NEAR:
            {
                plane.Point =  {0.0f, 0.0f, ZNear};
                plane.Normal = {0.0f, 0.0f, 1.0f};
                break;
            }
            case FRUSTUM_FAR:
            {
                plane.Point = {0.0f, 0.0f, ZFar};
                plane.Normal = {0.0f, 0.0f, -1.0f};
                break;
            }
            default:
                break;
        }
        return plane;
    }
};