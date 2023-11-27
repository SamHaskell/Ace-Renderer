#include "graphics/texture.hpp"

namespace Ace {
    Texture* Texture::Create(u32* data, u32 width, u32 height) {
        Texture* tex = new Texture(width, height);
        tex->Data = data;
        return tex;
    }

    Texture* Texture::Load(const std::string& path) {
        
    }
};