#include "graphics/color.hpp"

namespace Ace {
    u32 Color::U32_RGBA() {
        return ((u32)(a * 255.0f) << 24) 
        | ((u32)(b * 255.0f) << 16) 
        | ((u32)(g * 255.0f) << 8) 
        | (u32)(r * 255.0f);
    }
}