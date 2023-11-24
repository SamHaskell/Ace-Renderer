#include "graphics/graphicsdevice.hpp"
#include "maths/maths.hpp"

namespace Ace {
    void GraphicsDevice::DrawGrid(PixelBuffer& pixelBuffer, u32 color, u32 xStep, u32 yStep) {
        for (i32 i = 0; i < pixelBuffer.Width / 2; i+=xStep) {
            for (i32 j = 0; j < pixelBuffer.Height; j++) {
                pixelBuffer.SetPixel(pixelBuffer.Width / 2 + i, j, color);
                pixelBuffer.SetPixel(pixelBuffer.Width / 2 - i, j, color);
            }
        }

        for (i32 i = 0; i < pixelBuffer.Width; i++) {
            for (i32 j = 0; j < pixelBuffer.Height / 2; j+=yStep) {
                pixelBuffer.SetPixel(i, pixelBuffer.Height / 2 + j, color);
                pixelBuffer.SetPixel(i, pixelBuffer.Height / 2 - j, color);
            }
        }
    }

    void GraphicsDevice::DrawLine(PixelBuffer& pixelBuffer, u32 color, Vec2 start, Vec2 end) {
        i32 deltaX = (i32)(end.x - start.x);
        i32 deltaY = (i32)(end.y - start.y);

        f32 span =  fmax(static_cast<f32>(deltaX), static_cast<f32>(deltaY));

        f32 dx = deltaX / span;
        f32 dy = deltaY / span;

        for (i32 i = 0; i < (i32)span; i++) {
            pixelBuffer.SetPixel(round(i*dx + start.x), round(i*dy + start.y), color);
        }
    }

    void GraphicsDevice::DrawRect(PixelBuffer& pixelBuffer, u32 color, const Rect& rect) {
        if ((rect.w < 0) || (rect.h < 0)) {
            return;
        }
        for (i32 i = 0; i < rect.w; i++) {
            pixelBuffer.SetPixel(i + rect.x, rect.y, color);
            pixelBuffer.SetPixel(i + rect.x, rect.y + rect.h, color);
        }
        for (i32 j = 0; j < rect.h; j++) {
            pixelBuffer.SetPixel(rect.x, rect.y, color);
            pixelBuffer.SetPixel(rect.x + rect.w, rect.y, color);
        }
    }

    void GraphicsDevice::DrawRectFill(PixelBuffer& pixelBuffer, u32 color, const Rect& rect) {
        for (i32 i = 0; i < rect.w; i++) {
            for (i32 j = 0; j < rect.h; j++) {
                pixelBuffer.SetPixel(i + rect.x, j + rect.y, color);
            }
        }
    }
};