#include "graphics/graphicsdevice.hpp"
#include "graphics/triangle.hpp"
#include "graphics/color.hpp"
#include "graphics/texture.hpp"
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
            { triangle.Vertices[0].Position.x, triangle.Vertices[0].Position.y },
            { triangle.Vertices[1].Position.x, triangle.Vertices[1].Position.y }
        );

        DrawLine(
            pixelBuffer,
            color,
            { triangle.Vertices[1].Position.x, triangle.Vertices[1].Position.y },
            { triangle.Vertices[2].Position.x, triangle.Vertices[2].Position.y }
        );

        DrawLine(
            pixelBuffer,
            color,
            { triangle.Vertices[2].Position.x, triangle.Vertices[2].Position.y },
            { triangle.Vertices[0].Position.x, triangle.Vertices[0].Position.y }
        );
    }

    void GraphicsDevice::DrawTriangleFill(PixelBuffer& pixelBuffer, Color color, Triangle triangle) {
        // Sort Points top to bottom
        
        if (triangle.Vertices[0].Position.y > triangle.Vertices[1].Position.y) {
            Swap<Vertex>(triangle.Vertices[0], triangle.Vertices[1]);
        }

        if (triangle.Vertices[0].Position.y > triangle.Vertices[2].Position.y) {
            Swap<Vertex>(triangle.Vertices[0], triangle.Vertices[2]);
        }

        if (triangle.Vertices[1].Position.y > triangle.Vertices[2].Position.y) {
            Swap<Vertex>(triangle.Vertices[1], triangle.Vertices[2]);
        }

        if (triangle.Vertices[0].Position.y == triangle.Vertices[1].Position.y) {
            if (triangle.Vertices[0].Position.x > triangle.Vertices[1].Position.x) {
                Swap<Vertex>(triangle.Vertices[0], triangle.Vertices[1]);
            }
            DrawTriangleFlatTop(
                pixelBuffer, color, 
                triangle.Vertices[2], 
                triangle.Vertices[0], 
                triangle.Vertices[1]
            );
            return;
        } else if (triangle.Vertices[1].Position.y == triangle.Vertices[2].Position.y) {
            if (triangle.Vertices[1].Position.x > triangle.Vertices[2].Position.x) {
                Swap<Vertex>(triangle.Vertices[1], triangle.Vertices[2]);
            }
            DrawTriangleFlatBottom(
                pixelBuffer, color, 
                triangle.Vertices[2], 
                triangle.Vertices[1],
                triangle.Vertices[0]
            );
            return;
        }

        // Find the midpoint that splits triangle into flat top and flat bottom

        f32 t = Unlerp(triangle.Vertices[0].Position.y, triangle.Vertices[2].Position.y, triangle.Vertices[1].Position.y);
        f32 mx = Lerp(triangle.Vertices[0].Position.x, triangle.Vertices[2].Position.x, t);

        Vec2 midpoint = {mx, triangle.Vertices[1].Position.y};

        Vertex midVert = {
            .Position = {mx, triangle.Vertices[1].Position.y, 0.0f, 0.0f},
            .TexCoord = {0.5f, 0.5f}
        };

        // Draw flat top and flat bottom.

        if (midpoint.x > triangle.Vertices[1].Position.x) {
            DrawTriangleFlatTop(
                pixelBuffer, color, 
                triangle.Vertices[2], 
                triangle.Vertices[1], 
                midVert
            );

            DrawTriangleFlatBottom(
                pixelBuffer, color, 
                triangle.Vertices[0], 
                triangle.Vertices[1], 
                midVert
            );
        } else {
            DrawTriangleFlatTop(
                pixelBuffer, color, 
                triangle.Vertices[2], 
                midVert,
                triangle.Vertices[1]
            );

            DrawTriangleFlatBottom(
                pixelBuffer, color, 
                triangle.Vertices[0], 
                midVert,
                triangle.Vertices[1]
            );
        }
    }

    void GraphicsDevice::DrawTriangleTextured(PixelBuffer& pixelBuffer, const Texture& texture, Triangle triangle) {

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
        Vertex top,
        Vertex bottomLeft, 
        Vertex bottomRight
    ) {
        u32 uColor = color.U32_ARGB();

        f32 invSlopeLeft = (f32)(bottomLeft.Position.x - top.Position.x)/(bottomLeft.Position.y - top.Position.y);
        f32 invSlopeRight = (f32)(bottomRight.Position.x - top.Position.x)/(bottomRight.Position.y - top.Position.y);

        f32 startX = top.Position.x;
        f32 endX = top.Position.x;

        for (i32 y = top.Position.y; y <= bottomRight.Position.y; y++) {
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
        Vertex bottom,
        Vertex topLeft, 
        Vertex topRight 
    ) {
        u32 uColor = color.U32_ARGB();

        f32 invSlopeLeft = (f32)(bottom.Position.x - topLeft.Position.x)/(bottom.Position.y - topLeft.Position.y);
        f32 invSlopeRight = (f32)(bottom.Position.x - topRight.Position.x)/(bottom.Position.y - topRight.Position.y);

        f32 startX = bottom.Position.x;
        f32 endX = bottom.Position.x;

        for (i32 y = bottom.Position.y; y >= topRight.Position.y; y--) {
            for (i32 x = startX; x <= round(endX); x++) {
                pixelBuffer.SetPixel(x, y, uColor);
            }
            startX -= invSlopeLeft;
            endX -= invSlopeRight;
        }
    }

};