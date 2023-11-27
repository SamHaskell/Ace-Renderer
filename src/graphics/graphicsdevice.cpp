#include "graphics/graphicsdevice.hpp"
#include "graphics/triangle.hpp"
#include "graphics/color.hpp"
#include "maths/maths.hpp"

namespace Ace {
    void GraphicsDevice::DrawGrid(PixelBuffer& pixelBuffer, Color color, u32 xStep, u32 yStep) {
        u32 uColor = color.U32_ARGB();

        for (i32 i = 0; i < pixelBuffer.Width / 2; i+=xStep) {
            for (i32 j = 0; j < pixelBuffer.Height; j++) {
                pixelBuffer.SetPixel(pixelBuffer.Width / 2 + i, j, uColor);
                pixelBuffer.SetPixel(pixelBuffer.Width / 2 - i, j, uColor);
            }
        }

        for (i32 i = 0; i < pixelBuffer.Width; i++) {
            for (i32 j = 0; j < pixelBuffer.Height / 2; j+=yStep) {
                pixelBuffer.SetPixel(i, pixelBuffer.Height / 2 + j, uColor);
                pixelBuffer.SetPixel(i, pixelBuffer.Height / 2 - j, uColor);
            }
        }
    }

    void GraphicsDevice::DrawLine(PixelBuffer& pixelBuffer, Color color, Vec2 start, Vec2 end) {
        u32 uColor = color.U32_ARGB();

        i32 span = MAX(fabs(end.x - start.x), fabs(end.y - start.y));
        f32 dx = (f32)(end.x - start.x) / span;
        f32 dy = (f32)(end.y - start.y) / span;

        f32 x = start.x;
        f32 y = start.y;

        for (i32 i = 0; i < span; i++) {
            pixelBuffer.SetPixel(roundf(x), roundf(y), uColor);
            x += dx;
            y += dy;
        }
    }

    void GraphicsDevice::DrawTriangle(PixelBuffer& pixelBuffer, Color color, const Triangle& triangle) {
        DrawLine(
            pixelBuffer,
            color,
            triangle.Points[0],
            triangle.Points[1]
        );

        DrawLine(
            pixelBuffer,
            color,
            triangle.Points[1],
            triangle.Points[2]
        );

        DrawLine(
            pixelBuffer,
            color,
            triangle.Points[2],
            triangle.Points[0]
        );
    }

    void GraphicsDevice::DrawTriangleFill(PixelBuffer& pixelBuffer, Color color, Triangle triangle) {
        // Sort Points top to bottom
        
        if (triangle.Points[0].y > triangle.Points[1].y) {
            Swap<Vec2>(triangle.Points[0], triangle.Points[1]);
        }

        if (triangle.Points[0].y > triangle.Points[2].y) {
            Swap<Vec2>(triangle.Points[0], triangle.Points[2]);
        }

        if (triangle.Points[1].y > triangle.Points[2].y) {
            Swap<Vec2>(triangle.Points[1], triangle.Points[2]);
        }

        if (triangle.Points[0].y == triangle.Points[1].y) {
            if (triangle.Points[0].x > triangle.Points[1].x) {
                Swap<Vec2>(triangle.Points[0], triangle.Points[1]);
            }
            DrawTriangleFlatTop(
                pixelBuffer, color, 
                triangle.Points[2].x, triangle.Points[2].y, 
                triangle.Points[0].x, triangle.Points[0].y, 
                triangle.Points[1].x, triangle.Points[1].y
            );
            return;
        } else if (triangle.Points[1].y == triangle.Points[2].y) {
            if (triangle.Points[1].x > triangle.Points[2].x) {
                Swap<Vec2>(triangle.Points[1], triangle.Points[2]);
            }
            DrawTriangleFlatBottom(
                pixelBuffer, color, 
                triangle.Points[2].x, triangle.Points[2].y, 
                triangle.Points[1].x, triangle.Points[1].y,
                triangle.Points[0].x, triangle.Points[0].y
            );
            return;
        }

        // Find the midpoint that splits triangle into flat top and flat bottom

        f32 t = Unlerp(triangle.Points[0].y, triangle.Points[2].y, triangle.Points[1].y);
        f32 mx = Lerp(triangle.Points[0].x, triangle.Points[2].x, t);

        Vec2 midpoint = {mx, triangle.Points[1].y};

        // Draw flat top and flat bottom.

        if (midpoint.x > triangle.Points[1].x) {
            DrawTriangleFlatTop(
                pixelBuffer, color, 
                triangle.Points[2].x, triangle.Points[2].y, 
                triangle.Points[1].x, triangle.Points[1].y, 
                midpoint.x, midpoint.y
            );

            DrawTriangleFlatBottom(
                pixelBuffer, color, 
                triangle.Points[0].x, triangle.Points[0].y, 
                triangle.Points[1].x, triangle.Points[1].y, 
                midpoint.x, midpoint.y
            );
        } else {
            DrawTriangleFlatTop(
                pixelBuffer, color, 
                triangle.Points[2].x, triangle.Points[2].y, 
                midpoint.x, midpoint.y,
                triangle.Points[1].x, triangle.Points[1].y
            );

            DrawTriangleFlatBottom(
                pixelBuffer, color, 
                triangle.Points[0].x, triangle.Points[0].y, 
                midpoint.x, midpoint.y,
                triangle.Points[1].x, triangle.Points[1].y
            );
        }
    }

    void GraphicsDevice::DrawRect(PixelBuffer& pixelBuffer, Color color, const Rect& rect) {
        u32 uColor = color.U32_ARGB();

        if ((rect.w < 0) || (rect.h < 0)) {
            return;
        }
        for (i32 i = 0; i < rect.w; i++) {
            pixelBuffer.SetPixel(i + rect.x, rect.y, uColor);
            pixelBuffer.SetPixel(i + rect.x, rect.y + rect.h, uColor);
        }
        for (i32 j = 0; j < rect.h; j++) {
            pixelBuffer.SetPixel(rect.x, rect.y, uColor);
            pixelBuffer.SetPixel(rect.x + rect.w, rect.y, uColor);
        }
    }

    void GraphicsDevice::DrawRectFill(PixelBuffer& pixelBuffer, Color color, const Rect& rect) {
        u32 uColor = color.U32_ARGB();
        for (i32 i = 0; i < rect.w; i++) {
            for (i32 j = 0; j < rect.h; j++) {
                pixelBuffer.SetPixel(i + rect.x, j + rect.y, uColor);
            }
        }
    }


    void GraphicsDevice::DrawTriangleFlatBottom(
        PixelBuffer& pixelBuffer, 
        Color color, 
        i32 topX, i32 topY,
        i32 bottomLeftX, i32 bottomLeftY, 
        i32 bottomRightX, i32 bottomRightY
    ) {
        u32 uColor = color.U32_ARGB();

        f32 invSlopeLeft = (f32)(bottomLeftX - topX)/(bottomLeftY - topY);
        f32 invSlopeRight = (f32)(bottomRightX - topX)/(bottomRightY - topY);

        f32 startX = topX;
        f32 endX = topX;

        for (i32 y = topY; y <= bottomRightY; y++) {
            for (i32 x = startX; x <= round(endX); x++) {
                pixelBuffer.SetPixel(x, y, uColor);
            }
            startX += invSlopeLeft;
            endX += invSlopeRight;
        }
    }

    void GraphicsDevice::DrawTriangleFlatTop(
        PixelBuffer& pixelBuffer, 
        Color color,
        i32 bottomX, i32 bottomY,
        i32 topLeftX, i32 topLeftY, 
        i32 topRightX, i32 topRightY 
    ) {
        u32 uColor = color.U32_ARGB();

        f32 invSlopeLeft = (f32)(bottomX - topLeftX)/(bottomY - topLeftY);
        f32 invSlopeRight = (f32)(bottomX - topRightX)/(bottomY - topRightY);

        f32 startX = bottomX;
        f32 endX = bottomX;

        for (i32 y = bottomY; y >= topRightY; y--) {
            for (i32 x = startX; x <= round(endX); x++) {
                pixelBuffer.SetPixel(x, y, uColor);
            }
            startX -= invSlopeLeft;
            endX -= invSlopeRight;
        }
    }

};