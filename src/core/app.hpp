#pragma once

#include "core/defines.hpp"
#include "graphics/pixelbuffer.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace Ace {
    class App {
        public:
            App() = default;
            virtual ~App() = default;
            void Run();
            virtual void Initialise() = 0;
            virtual void Update(f64 dt) = 0;
            virtual void Render(PixelBuffer& pixelBuffer) = 0;
            virtual void OnEvent() = 0;

        private:
            bool m_Running;
            SDL_Window* m_Window;
            SDL_Renderer* m_Renderer;
            PixelBuffer* m_PixelBuffer;
            SDL_Texture* m_PixelBufferTexture;

            void Tick();
    };
};