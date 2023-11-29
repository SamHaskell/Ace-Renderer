#pragma once

#include "core/defines.hpp"
#include "maths/vector.hpp"

namespace Ace {
    struct Vec3;
    struct Vec4;

    struct Mat4 {
        f32 m[16];
        inline f32 operator[](size_t pos) const { return m[pos]; }
        inline f32& operator[](size_t pos) { return m[pos]; }

        Vec4 operator* (const Vec4& v);
        Vec4 operator* (const Vec3& v);
        Mat4 operator* (const Mat4& other);
    
        static Mat4 Identity();
        static Mat4 Scale(Vec3 scale);
        static Mat4 Rotation(Vec3 euler);
        static Mat4 Translation(Vec3 translation);

        static Mat4 Perspective(f32 fovY, f32 aspect, f32 zNear, f32 zFar);

        static Mat4 LookAt(Vec3 eye, Vec3 target, Vec3 up = {0.0f, 1.0f, 0.0f});
    };
};