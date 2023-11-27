#include "graphics/triangle.hpp"

namespace Ace {
    Vec3 Triangle::BarycentricWeights(Vec2 point) {
        Vec2 a = { Vertices[0].Position.x, Vertices[0].Position.y };
        Vec2 b = { Vertices[1].Position.x, Vertices[1].Position.y };
        Vec2 c = { Vertices[2].Position.x, Vertices[2].Position.y };

        f32 areaABC = Span(c - a, b - a);
        f32 alpha = Span(c - point, b - point) / areaABC;
        f32 beta = Span(c - a, point - a) / areaABC;
        Vec3 out = { alpha, beta, 1.0f - alpha - beta};

        return out;
    }

    Vec2 Triangle::InterpolatedUV(Vec3 barycentricWeights) {

        f32 interpolatedInvW = (
            (1.0f / Vertices[0].Position.w) * barycentricWeights.x +
            (1.0f / Vertices[1].Position.w) * barycentricWeights.y +
            (1.0f / Vertices[2].Position.w) * barycentricWeights.z
        );

        Vec2 interpolatedUV =   Vertices[0].TexCoord * (1.0f / Vertices[0].Position.w) * barycentricWeights.x +
                                Vertices[1].TexCoord * (1.0f / Vertices[1].Position.w) * barycentricWeights.y +
                                Vertices[2].TexCoord * (1.0f / Vertices[2].Position.w) * barycentricWeights.z;

        interpolatedUV *= (1.0f / interpolatedInvW);

        return interpolatedUV;
    }
};