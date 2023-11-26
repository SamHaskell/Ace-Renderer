#include "maths/matrix.hpp"
#include "maths/vector.hpp"

namespace Ace {
    Vec4 Mat4::operator* (const Vec4& v) {
        return {
            m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * v.w,
            m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * v.w,
            m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * v.w,
            m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * v.w 
        };
    }

    Vec4 Mat4::operator* (const Vec3& v) {
        return {
            m[0] * v.x + m[4] * v.y + m[8] * v.z + m[12] * 1.0f,
            m[1] * v.x + m[5] * v.y + m[9] * v.z + m[13] * 1.0f,
            m[2] * v.x + m[6] * v.y + m[10] * v.z + m[14] * 1.0f,
            m[3] * v.x + m[7] * v.y + m[11] * v.z + m[15] * 1.0f 
        };
    }

    Mat4 Mat4::operator* (const Mat4& other) {
        return {
            m[0] * other[0] + m[4] * other[1] + m[8] * other[2] + m[12] * other[3],
            m[0] * other[4] + m[4] * other[5] + m[8] * other[6] + m[12] * other[7],
            m[0] * other[8] + m[4] * other[9] + m[8] * other[10] + m[12] * other[11],
            m[0] * other[12] + m[4] * other[13] + m[8] * other[14] + m[12] * other[15],

            m[1] * other[0] + m[5] * other[1] + m[9] * other[2] + m[13] * other[3],
            m[1] * other[4] + m[5] * other[5] + m[9] * other[6] + m[13] * other[7],
            m[1] * other[8] + m[5] * other[9] + m[9] * other[10] + m[13] * other[11],
            m[1] * other[12] + m[5] * other[13] + m[9] * other[14] + m[13] * other[15],

            m[2] * other[0] + m[6] * other[1] + m[10] * other[2] + m[14] * other[3],
            m[2] * other[4] + m[6] * other[5] + m[10] * other[6] + m[14] * other[7],
            m[2] * other[8] + m[6] * other[9] + m[10] * other[10] + m[14] * other[11],
            m[2] * other[12] + m[6] * other[13] + m[10] * other[14] + m[14] * other[15],

            m[3] * other[0] + m[7] * other[1] + m[11] * other[2] + m[15] * other[3],
            m[3] * other[4] + m[7] * other[5] + m[11] * other[6] + m[15] * other[7],
            m[3] * other[8] + m[7] * other[9] + m[11] * other[10] + m[15] * other[11],
            m[3] * other[12] + m[7] * other[13] + m[11] * other[14] + m[15] * other[15]
        };
    }

    Mat4 Mat4::Identity() {
        return {
            {
                1.0f, 0.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 1.0f
            }
        };
    }

    Mat4 Mat4::Scale(Vec3 scale) {
        Mat4 m = Mat4::Identity();
        m[0] = scale.x;
        m[5] = scale.y;
        m[10] = scale.z;
        return m;
    }

    Mat4 Mat4::Rotation(Vec3 euler) {
        Mat4 m = Mat4::Identity();
        
        f32 sinX = sinf(DEG2RAD * euler.x);
        f32 cosX = cosf(DEG2RAD * euler.x);
        f32 sinY = sinf(DEG2RAD * euler.y);
        f32 cosY = cosf(DEG2RAD * euler.y);
        f32 sinZ = sinf(DEG2RAD * euler.z);
        f32 cosZ = cosf(DEG2RAD * euler.z);

        m[0]  = + (cosY * cosZ);
        m[4]  = - (cosY * sinZ);
        m[8]  = + (sinY);
        
        m[1]  = + (sinX * sinY * cosZ) + (cosX * sinZ);
        m[5]  = - (sinX * sinY * sinZ) + (cosX * cosZ); 
        m[9]  = - (sinX * cosY);

        m[2]  = - (cosX * sinY * cosZ) + (sinX * sinZ);
        m[6]  = + (cosX * sinY * sinZ) + (sinX * cosZ);
        m[10] = + (cosX * cosY);

        return m;
    }

    Mat4 Mat4::Translation(Vec3 translation) {
        Mat4 m = Mat4::Identity();
        m[12] = translation.x;
        m[13] = translation.y;
        m[14] = translation.z;
        return m;
    }

    Mat4 Mat4::Perspective(f32 fovY, f32 aspect, f32 zNear, f32 zFar) {
        Mat4 m = Mat4::Identity();
        f32 invTan = 1.0f / tanf(DEG2RAD * fovY / 2.0f);

        m[0] = (1.0f / aspect) * invTan;
        m[5] = invTan;
        m[10] = - (zFar + zNear) / (zFar - zNear);
        m[14] = - 2.0f * zFar * zNear / (zFar - zNear);
        m[11] = - 1.0f;
        m[15] = 0.0f;

        return m;
    }

}