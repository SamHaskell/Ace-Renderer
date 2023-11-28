#pragma once

#include "core/defines.hpp"
#include "graphics/pixelbuffer.hpp"

namespace Ace {
    struct Rect;
    struct Vec2;
    struct Triangle;
    struct Color;
    struct Texture;
    struct Vertex;
    
    class GraphicsDevice {
        public:
            GraphicsDevice() = default;
            ~GraphicsDevice() = default;

            static void DrawGrid(PixelBuffer& pixelBuffer, Color color, u32 xStep, u32 yStep);

            static void DrawLine(PixelBuffer& pixelBuffer, Color color, Vec2 start, Vec2 end);

            static void DrawTriangle(PixelBuffer& pixelBuffer, DepthBuffer& depthBuffer, Color color, const Triangle& triangle);
            static void DrawTriangleFill(PixelBuffer& pixelBuffer, DepthBuffer& depthBuffer, Color color, Triangle triangle);
            static void DrawTriangleTextured(PixelBuffer& pixelBuffer, DepthBuffer& depthBuffer, const Texture& texture, Triangle triangle);

            static void DrawRect(PixelBuffer& pixelBuffer, Color color, const Rect& rect);
            static void DrawRectFill(PixelBuffer& pixelBuffer, Color color, const Rect& rect);
    };
}