#pragma once

#include "core/defines.hpp"
#include "graphics/pixelbuffer.hpp"

namespace Ace {
    struct Rect;
    struct Vec2;
    struct Triangle;
    
    class GraphicsDevice {
        public:
            GraphicsDevice() = default;
            ~GraphicsDevice() = default;

            static void DrawGrid(PixelBuffer& pixelBuffer, u32 color, u32 xStep, u32 yStep);

            static void DrawLine(PixelBuffer& pixelBuffer, u32 color, Vec2 start, Vec2 end);

            static void DrawTriangle(PixelBuffer& pixelBuffer, u32 color, const Triangle& triangle);
            static void DrawTriangleFill(PixelBuffer& pixelBuffer, u32 color, const Triangle& triangle);

            static void DrawRect(PixelBuffer& pixelBuffer, u32 color, const Rect& rect);
            static void DrawRectFill(PixelBuffer& pixelBuffer, u32 color, const Rect& rect);
        
        private:
            static void DrawTriangleFlatBottom(PixelBuffer& pixelBuffer, u32 color, Vec2 top, Vec2 bottomLeft, Vec2 bottomRight);
            static void DrawTriangleFlatTop(PixelBuffer& pixelBuffer, u32 color, Vec2 bottom, Vec2 topLeft, Vec2 topRight);
    };
}