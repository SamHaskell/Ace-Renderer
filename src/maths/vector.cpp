#include "maths/vector.hpp"
#include "maths/numerics.hpp"

namespace Ace {
    f32 SqrMagnitude(const Vec2& v) {
        return Dot(v, v);
    }

    f32 Magnitude(const Vec2& v) {
        return sqrt(Dot(v, v));
    }

    Vec2 Normalised(const Vec2& v) {
        f32 inv = 1.0f/Magnitude(v);
        return v * inv;
    }

    f32 Dot(const Vec2& u, const Vec2& v) {
        return u.x*v.x + u.y*v.y;
    }

    Vec2 Hadamard(const Vec2& u, const Vec2& v) {
        return {u.x*v.x, u.y*v.y};
    }

    Vec2 Lerp(const Vec2& u, const Vec2& v, f32 t) {
        return { Lerp(u.x, v.x, t), Lerp(u.y, v.y, t) };
    }

    Vec2 LerpClamped(const Vec2& u, const Vec2& v, f32 t) {
        f32 clamped = Clamp(t, 0.0f, 1.0f);
        return { Lerp(u.x, v.x, clamped), Lerp(u.y, v.y, clamped) };
    }

    f32 Span(const Vec2& u, const Vec2& v) {
        return u.x * v.y - v.x * u.y;
    }

    f32 SqrMagnitude(const Vec3& v) {
        return Dot(v, v);
    }

    f32 Magnitude(const Vec3& v) {
        return sqrt(Dot(v, v));
    }

    Vec3 Normalised(const Vec3& v) {
        f32 inv = 1.0f/Magnitude(v);
        return v * inv;
    }

    f32 Dot(const Vec3& u, const Vec3& v) {
        return u.x*v.x + u.y*v.y + u.z*v.z;
    }

    Vec3 Hadamard(const Vec3& u, const Vec3& v) {
        return Vec3(u.x*v.x, u.y*v.y, u.z*v.z);
    }

    Vec3 Lerp(const Vec3& u, const Vec3& v, f32 t) {
        return Vec3( Lerp(u.x, v.x, t), Lerp(u.y, v.y, t), Lerp(u.z, v.z, t) );
    }

    Vec3 LerpClamped(const Vec3& u, const Vec3& v, f32 t) {
        f32 clamped = Clamp(t, 0.0f, 1.0f);
        return Vec3( Lerp(u.x, v.x, clamped), Lerp(u.y, v.y, clamped), Lerp(u.z, v.z, clamped) );
    }

    Vec3 Cross(const Vec3& u, const Vec3& v) {
        return Vec3(
            u.y*v.z - u.z*v.y,
            u.z*v.x - u.x*v.z,
            u.x*v.y - u.y*v.x
        );
    }

    Vec3::Vec3(Vec4 vec) : x(vec.x), y(vec.y), z(vec.z) {}

    Vec4 Lerp(const Vec4& u, const Vec4& v, f32 t) {
        return Vec4( Lerp(u.x, v.x, t), Lerp(u.y, v.y, t), Lerp(u.z, v.z, t), Lerp(u.w, v.w, t) );
    }
}