#pragma once

#include "core/defines.hpp"
#include "graphics/color.hpp"

namespace Ace {
    class Texture {
        public:
            static Texture* Create(u32* data, u32 width, u32 height);
            static Texture* Create(u8* byteData, u32 width, u32 height);
            static Texture* Load(const std::string& path);

            u32 Sample(f32 u, f32 v);
        
            Texture(u32 width, u32 height) : Width(width), Height(height) {};
            ~Texture() = default;

            Texture(const Texture&) = delete;
            Texture& operator=(Texture const&) = delete;

        private:
            u32* Data;
            const u32 Width;
            const u32 Height;
    };
}