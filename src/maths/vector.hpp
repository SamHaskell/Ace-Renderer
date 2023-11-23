#pragma once

#include "core/defines.hpp"

namespace Ace {
    struct Vec2 {
        f32 x;
        f32 y;

        void operator*=(const f32 val) { x *= val; y *= val; }
        void operator*=(const Vec2& v) { x *= v.x; y *= v.y; }
        Vec2 operator* (const f32 val) const { return { x * val, y * val }; }
        Vec2 operator* (const Vec2& v) const { return { x * v.x, y * v.y }; }
        void operator+=(const f32 val) { x += val; y += val; }
        void operator+=(const Vec2& v) { x += v.x; y += v.y; }
        Vec2 operator+ (const f32 val) const { return { x + val, y + val }; }
        Vec2 operator+ (const Vec2& v) const { return { x + v.x, y + v.y }; }
        void operator-=(const f32 val) { x -= val; y -= val; }
        void operator-=(const Vec2& v) { x -= v.x; y -= v.y; }
        Vec2 operator- (const f32 val) const { return { x - val, y - val }; }
        Vec2 operator- (const Vec2& v) const { return { x - v.x, y - v.y }; }
        
        void AddScaledVector(const Vec2& vec, f32 scale) {
            x += vec.x * scale;
            y += vec.y * scale;
        }
        
        void HadamardInPlace(const Vec2& other) {
            x *= other.x;
            y *= other.y;
        }
        
        void NormaliseInPlace() {
            f32 inv = 1.0f/(x*x + y*y);
            x *= inv;
            y *= inv;
        }

        static Vec2 Up() { return {0.0, 1.0}; }
        static Vec2 Down() { return {0.0, -1.0}; }
        static Vec2 Right() { return {1.0, 0.0}; }
        static Vec2 Left() { return {-1.0, 0.0}; }
    };

    inline Vec2 operator- (const Vec2& v) { return {-v.x, -v.y}; }

    f32 SqrMagnitude(const Vec2& v);
    f32 Magnitude(const Vec2& v);
    Vec2 Normalised(const Vec2& v);
    f32 Dot(const Vec2& u, const Vec2& v);
    Vec2 Hadamard(const Vec2& u, const Vec2& v);
    Vec2 Lerp(const Vec2& u, const Vec2& v, f32 t);
    Vec2 LerpClamped(const Vec2& u, const Vec2& v, f32 t);

    struct Vec3 {
        f32 x;
        f32 y;
        f32 z;

        void operator*=(const f32 val) { x *= val; y *= val; z *= val; }
        void operator*=(const Vec3& v) { x *= v.x; y *= v.y; z *= v.z; }
        Vec3 operator* (const f32 val) const { return { x * val, y * val, z * val }; }
        Vec3 operator* (const Vec3& v) const { return { x * v.x, y * v.y, z * v.z }; }
        void operator+=(const f32 val) { x += val; y += val; z += val; }
        void operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; }
        Vec3 operator+ (const f32 val) const { return { x + val, y + val, z + val }; }
        Vec3 operator+ (const Vec3& v) const { return { x + v.x, y + v.y, z + v.z }; }
        void operator-=(const f32 val) { x -= val; y -= val; z -= val; }
        void operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; }
        Vec3 operator- (const f32 val) const { return { x - val, y - val, z - val }; }
        Vec3 operator- (const Vec3& v) const { return { x - v.x, y - v.y, z - v.z }; }

        void AddScaledVector(const Vec3& vec, f32 scale) {
            x += vec.x * scale;
            y += vec.y * scale;
            z += vec.z * scale;
        }
        
        void HadamardInPlace(const Vec3& other) {
            x *= other.x;
            y *= other.y;
            z *= other.z;
        }

        void NormaliseInPlace() {
            f32 inv = 1.0f/(x*x + y*y + z*z);
            x *= inv;
            y *= inv;
            z *= inv;
        }

        static Vec3 Up() { return {0.0, 1.0, 0.0}; }
        static Vec3 Down() { return {0.0, -1.0, 0.0}; }
        static Vec3 Right() { return {1.0, 0.0, 0.0}; }
        static Vec3 Left() { return {-1.0, 0.0, 0.0}; }
        static Vec3 Forward() { return {0.0, 0.0, 1.0}; }
        static Vec3 Back() { return {0.0, 0.0, -1.0}; }
    };

    inline Vec3 operator- (const Vec3& v) { return {-v.x, -v.y, -v.z}; }

    f32 SqrMagnitude(const Vec3& v);
    f32 Magnitude(const Vec3& v);
    Vec3 Normalised(const Vec3& v);
    f32 Dot(const Vec3& u, const Vec3& v);
    Vec3 Hadamard(const Vec3& u, const Vec3& v);
    Vec3 Lerp(const Vec3& u, const Vec3& v, f32 t);
    Vec3 LerpClamped(const Vec3& u, const Vec3& v, f32 t);
    Vec3 Cross(const Vec3& u, const Vec3& v);

    struct Vec4 {
        f32 x;
        f32 y;
        f32 z;
        f32 w;
    };
};