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

    Vec3 Triangle::InterpolatedUVW(Vec3 barycentricWeights) {

        f32 interpolatedInvW = (
            (1.0f / Vertices[0].Position.w) * barycentricWeights.x +
            (1.0f / Vertices[1].Position.w) * barycentricWeights.y +
            (1.0f / Vertices[2].Position.w) * barycentricWeights.z
        );

        Vec3 interpolatedUVW = {
            Vertices[0].TexCoord.x * (1.0f / Vertices[0].Position.w) * barycentricWeights.x / interpolatedInvW +
            Vertices[1].TexCoord.x * (1.0f / Vertices[1].Position.w) * barycentricWeights.y / interpolatedInvW +
            Vertices[2].TexCoord.x * (1.0f / Vertices[2].Position.w) * barycentricWeights.z / interpolatedInvW,

            Vertices[0].TexCoord.y * (1.0f / Vertices[0].Position.w) * barycentricWeights.x / interpolatedInvW +
            Vertices[1].TexCoord.y * (1.0f / Vertices[1].Position.w) * barycentricWeights.y / interpolatedInvW +
            Vertices[2].TexCoord.y * (1.0f / Vertices[2].Position.w) * barycentricWeights.z / interpolatedInvW,

            interpolatedInvW
        };

        return interpolatedUVW;
    }
};