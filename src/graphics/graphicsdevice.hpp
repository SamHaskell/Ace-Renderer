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

            static void DrawTriangle(PixelBuffer& pixelBuffer, Color color, const Triangle& triangle);
            static void DrawTriangleFill(PixelBuffer& pixelBuffer, Color color, Triangle triangle);
            static void DrawTriangleTextured(PixelBuffer& pixelBuffer, const Texture& texture, Triangle triangle);

            static void DrawRect(PixelBuffer& pixelBuffer, Color color, const Rect& rect);
            static void DrawRectFill(PixelBuffer& pixelBuffer, Color color, const Rect& rect);
        
        private:
            static void DrawTriangleFlatBottom(
                PixelBuffer& pixelBuffer, 
                Color color, 
                Vertex top,
                Vertex bottomLeft, 
                Vertex bottomRight
            );
            
            static void DrawTriangleFlatTop(
                PixelBuffer& pixelBuffer, 
                Color color,
                Vertex bottom,
                Vertex topLeft, 
                Vertex topRight 
            );

            static void DrawTriangleFlatBottomTextured(
                PixelBuffer& pixelBuffer,
                const Texture& texture,
                Vertex top,
                Vertex bottomLeft,
                Vertex bottomRight
            );

            static void DrawTriangleFlatTopTextured(
                PixelBuffer& pixelBuffer,
                const Texture& texture,
                Vertex bottom,
                Vertex topLeft,
                Vertex topRight
            );
    };
}