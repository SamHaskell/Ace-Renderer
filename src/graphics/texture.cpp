#include "graphics/texture.hpp"
#include "maths/maths.hpp"

namespace Ace {
    Texture* Texture::Create(u32* data, u32 width, u32 height) {
        Texture* tex = new Texture(width, height);
        tex->Data = data;
        return tex;
    }

    Texture* Texture::Create(u8* byteData, u32 width, u32 height) {
        Texture* tex = new Texture(width, height);
        tex->Data = (u32*)byteData;
        return tex;
    }

    Texture* Texture::Load(const std::string& path) {
        
    }

    u32 Texture::Sample(f32 u, f32 v) const {
        u32 row = Clamp(v, 0.0f, 1.0f) * Height;
        u32 col = Clamp(u, 0.0f, 1.0f) * Width;
        return Data[(row * Width) + col];
    }
};