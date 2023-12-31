#pragma once

#include "core/defines.hpp"
#include "core/events.hpp"
#include "graphics/pixelbuffer.hpp"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace Ace {
    class Clock {
        public:
            Clock() = default;
            ~Clock() = default;

            void Start() {
                m_Now = std::chrono::high_resolution_clock::now();
            }

            f64 Tick() {
                auto next = std::chrono::high_resolution_clock::now();
                f64 dt = std::chrono::duration<f64, std::chrono::seconds::period>(next - m_Now).count();
                m_Now = next;
                return dt;
            }

        private:
            std::chrono::steady_clock::time_point m_Now = std::chrono::high_resolution_clock::now();
    };

    class App {
        public:
            App() = default;
            virtual ~App() = default;
            void Run();
            virtual void Initialise() = 0;
            virtual void Shutdown() = 0;
            virtual void Update(f64 dt) = 0;
            virtual void Render(PixelBuffer& pixelBuffer, DepthBuffer& depthBuffer) = 0;
            virtual void DrawGUI() = 0;
            virtual void OnEvent(Event& e) = 0;

        private:
            bool m_Running;
            Clock m_MainClock;
            SDL_Window* m_Window;
            SDL_Renderer* m_Renderer;
            PixelBuffer* m_PixelBuffer;
            DepthBuffer* m_DepthBuffer;
            SDL_Texture* m_PixelBufferTexture;

            void Tick();
    };
};