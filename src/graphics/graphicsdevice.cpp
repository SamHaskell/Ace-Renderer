#include "graphics/graphicsdevice.hpp"
#include "graphics/triangle.hpp"
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
        i32 span = MAX(fabs(end.x - start.x), fabs(end.x - start.x));
        f32 dx = (f32)(end.x - start.x) / span;
        f32 dy = (f32)(end.y - start.y) / span;

        f32 x = start.x;
        f32 y = start.y;

        for (i32 i = 0; i < span; i++) {
            pixelBuffer.SetPixel(roundf(x), roundf(y), color);
            x += dx;
            y += dy;
        }
    }

    void GraphicsDevice::DrawTriangle(PixelBuffer& pixelBuffer, u32 color, const Triangle& triangle) {
        DrawLine(
            pixelBuffer,
            color,
            triangle.points[0],
            triangle.points[1]
        );

        DrawLine(
            pixelBuffer,
            color,
            triangle.points[1],
            triangle.points[2]
        );

        DrawLine(
            pixelBuffer,
            color,
            triangle.points[2],
            triangle.points[0]
        );
    }

    void GraphicsDevice::DrawTriangleFill(PixelBuffer& pixelBuffer, u32 color, Triangle triangle) {
        // Sort points top to bottom
        
        if (triangle.points[0].y > triangle.points[1].y) {
            Swap<Vec2>(triangle.points[0], triangle.points[1]);
        }

        if (triangle.points[0].y > triangle.points[2].y) {
            Swap<Vec2>(triangle.points[0], triangle.points[2]);
        }

        if (triangle.points[1].y > triangle.points[2].y) {
            Swap<Vec2>(triangle.points[1], triangle.points[2]);
        }

        if (triangle.points[0].y == triangle.points[1].y) {
            if (triangle.points[0].x > triangle.points[1].x) {
                Swap<Vec2>(triangle.points[0], triangle.points[1]);
            }
            DrawTriangleFlatTop(
                pixelBuffer, color, 
                triangle.points[2].x, triangle.points[2].y, 
                triangle.points[0].x, triangle.points[0].y, 
                triangle.points[1].x, triangle.points[1].y
            );
            return;
        } else if (triangle.points[1].y == triangle.points[2].y) {
            if (triangle.points[1].x > triangle.points[2].x) {
                Swap<Vec2>(triangle.points[1], triangle.points[2]);
            }
            DrawTriangleFlatBottom(
                pixelBuffer, color, 
                triangle.points[2].x, triangle.points[2].y, 
                triangle.points[1].x, triangle.points[1].y,
                triangle.points[0].x, triangle.points[0].y
            );
            return;
        }

        // Find the midpoint that splits triangle into flat top and flat bottom

        f32 t = Unlerp(triangle.points[0].y, triangle.points[2].y, triangle.points[1].y);
        f32 mx = Lerp(triangle.points[0].x, triangle.points[2].x, t);

        Vec2 midpoint = {mx, triangle.points[1].y};

        // Draw flat top and flat bottom.

        if (midpoint.x > triangle.points[1].x) {
            DrawTriangleFlatTop(
                pixelBuffer, color, 
                triangle.points[2].x, triangle.points[2].y, 
                triangle.points[1].x, triangle.points[1].y, 
                midpoint.x, midpoint.y
            );

            DrawTriangleFlatBottom(
                pixelBuffer, color, 
                triangle.points[0].x, triangle.points[0].y, 
                triangle.points[1].x, triangle.points[1].y, 
                midpoint.x, midpoint.y
            );
        } else {
            DrawTriangleFlatTop(
                pixelBuffer, color, 
                triangle.points[2].x, triangle.points[2].y, 
                midpoint.x, midpoint.y,
                triangle.points[1].x, triangle.points[1].y
            );

            DrawTriangleFlatBottom(
                pixelBuffer, color, 
                triangle.points[0].x, triangle.points[0].y, 
                midpoint.x, midpoint.y,
                triangle.points[1].x, triangle.points[1].y
            );
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


    void GraphicsDevice::DrawTriangleFlatBottom(
        PixelBuffer& pixelBuffer, 
        u32 color, 
        i32 topX, i32 topY,
        i32 bottomLeftX, i32 bottomLeftY, 
        i32 bottomRightX, i32 bottomRightY
    ) {
        f32 invSlopeLeft = (f32)(bottomLeftX - topX)/(bottomLeftY - topY);
        f32 invSlopeRight = (f32)(bottomRightX - topX)/(bottomRightY - topY);

        f32 startX = topX;
        f32 endX = topX;

        for (i32 y = topY; y <= bottomRightY; y++) {
            for (i32 x = startX; x <= round(endX); x++) {
                pixelBuffer.SetPixel(x, y, color);
            }
            startX += invSlopeLeft;
            endX += invSlopeRight;
        }
    }

    void GraphicsDevice::DrawTriangleFlatTop(
        PixelBuffer& pixelBuffer, 
        u32 color,
        i32 bottomX, i32 bottomY,
        i32 topLeftX, i32 topLeftY, 
        i32 topRightX, i32 topRightY 
    ) {
        f32 invSlopeLeft = (f32)(bottomX - topLeftX)/(bottomY - topLeftY);
        f32 invSlopeRight = (f32)(bottomX - topRightX)/(bottomY - topRightY);

        f32 startX = bottomX;
        f32 endX = bottomX;

        for (i32 y = bottomY; y >= topRightY; y--) {
            for (i32 x = startX; x <= round(endX); x++) {
                pixelBuffer.SetPixel(x, y, color);
            }
            startX -= invSlopeLeft;
            endX -= invSlopeRight;
        }
    }

};