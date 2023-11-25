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
            static void DrawTriangleFill(PixelBuffer& pixelBuffer, u32 color, Triangle triangle);

            static void DrawRect(PixelBuffer& pixelBuffer, u32 color, const Rect& rect);
            static void DrawRectFill(PixelBuffer& pixelBuffer, u32 color, const Rect& rect);
        
        private:
            static void DrawTriangleFlatBottom(
                PixelBuffer& pixelBuffer, 
                u32 color, 
                i32 topX, i32 topY,
                i32 bottomLeftX, i32 bottomLeftY, 
                i32 bottomRightX, i32 bottomRightY
            );
            
            static void DrawTriangleFlatTop(
                PixelBuffer& pixelBuffer, 
                u32 color,
                i32 bottomX, i32 bottomY,
                i32 topLeftX, i32 topLeftY, 
                i32 topRightX, i32 topRightY 
            );
    };
}