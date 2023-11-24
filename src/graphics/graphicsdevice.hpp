#pragma once

#include "core/defines.hpp"
#include "graphics/pixelbuffer.hpp"

namespace Ace {
    struct Rect;
    struct Vec2;
    
    class GraphicsDevice {
        public:
            GraphicsDevice() = default;
            ~GraphicsDevice() = default;

            static void DrawGrid(PixelBuffer& pixelBuffer, u32 color, u32 xStep, u32 yStep);

            static void DrawLine(PixelBuffer& pixelBuffer, u32 color, Vec2 start, Vec2 end);

            static void DrawRect(PixelBuffer& pixelBuffer, u32 color, const Rect& rect);
            static void DrawRectFill(PixelBuffer& pixelBuffer, u32 color, const Rect& rect);
        private:
    };
}