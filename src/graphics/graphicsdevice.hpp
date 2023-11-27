#pragma once

#include "core/defines.hpp"
#include "graphics/pixelbuffer.hpp"

namespace Ace {
    struct Rect;
    struct Vec2;
    struct Triangle;
    struct Color;
    struct Texture;
    
    class GraphicsDevice {
        public:
            GraphicsDevice() = default;
            ~GraphicsDevice() = default;

            static void DrawGrid(PixelBuffer& pixelBuffer, Color color, u32 xStep, u32 yStep);

            static void DrawLine(PixelBuffer& pixelBuffer, Color color, Vec2 start, Vec2 end);

            static void DrawTriangle(PixelBuffer& pixelBuffer, Color color, const Triangle& triangle);
            static void DrawTriangleFill(PixelBuffer& pixelBuffer, Color color, Triangle triangle);
            static void DrawTriangleTextured(PixelBuffer& pixelBuffer, const Texture& texture, Triangle triangle);

            static void DrawRect(PixelBuffer& pixelBuffer, Color color, const Rect& rect);
            static void DrawRectFill(PixelBuffer& pixelBuffer, Color color, const Rect& rect);
        
        private:
            static void DrawTriangleFlatBottom(
                PixelBuffer& pixelBuffer, 
                Color color, 
                i32 topX, i32 topY,
                i32 bottomLeftX, i32 bottomLeftY, 
                i32 bottomRightX, i32 bottomRightY
            );
            
            static void DrawTriangleFlatTop(
                PixelBuffer& pixelBuffer, 
                Color color,
                i32 bottomX, i32 bottomY,
                i32 topLeftX, i32 topLeftY, 
                i32 topRightX, i32 topRightY 
            );
    };
}