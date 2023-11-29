#include "graphics/texture.hpp"
#include "maths/maths.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>

/*
    TODO: Switch to a reference counted type / shared ptr for resources.
*/

namespace Ace {
    Texture::~Texture() {
        if (Data != NULL) {
            delete[] Data;
        }
    }

    Texture* Texture::Create(u32* data, u32 width, u32 height) {
        Texture* tex = new Texture(width, height);
        tex->Data = data;
        return tex;
    }

    Texture* Texture::Load(const std::string& path) {
        stbi_set_flip_vertically_on_load(true);
        i32 width, height, nChannels;
        u8* data = stbi_load(path.c_str(), &width, &height, &nChannels, 4);
        Texture* tex = new Texture(width, height);
        tex->Data = (u32*)data;
        return tex;
    }

    u32 Texture::Sample(f32 u, f32 v) const {
        u32 row = (u32)(v * Height) % Height;
        u32 col = (u32)(u * Width) % Width;
        return Data[(row * Width) + col];
    }
};