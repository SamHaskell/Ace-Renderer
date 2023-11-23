#pragma once

#include "core/defines.hpp"
#include "graphics/pixelbuffer.hpp"

namespace Ace {
    class GraphicsDevice {
        public:
            GraphicsDevice() = default;
            ~GraphicsDevice() = default;

            static void DrawGrid(PixelBuffer& pixelBuffer, u32 color, u32 xStep, u32 yStep);
        private:
    };
}