#pragma once

#include "core/defines.hpp"

namespace Ace {

    /*
        A simple 2D buffer of u32 pixel data. Width and Height are const as if the user needs to resize the buffer,
        this should be done by simply deleting and creating a new one.
    */

    struct PixelBuffer {
        u32* Data;
        const u32 Width;
        const u32 Height;

        PixelBuffer(u32 width, u32 height) : Width(width), Height(height) {
            Data = new u32[Width * Height];
        }

        ~PixelBuffer() {
            delete[] Data;
        }
        
        PixelBuffer(const PixelBuffer&) = delete;
        PixelBuffer& operator=(PixelBuffer const&) = delete;

        /*
            Sets the pixel at (i, j) to the specified value. If specified coordinates are out of bounds, do nothing.
        */

        inline void SetPixel(u32 i, u32 j, u32 value) {
            if ((i < Width) && (j < Height)) {
                Data[j * Width + i] = value;
                return;
            }
        }

        inline void Clear(u32 value) {
            for (i32 i = 0; i < Width * Height; i++) {
                Data[i] = value;
            }
        }
    };
};