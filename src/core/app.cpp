#include "core/app.hpp"

#include "SDL.h"
#include <chrono>

// TODO: Platform abstractions.

namespace Ace {
    void App::Run() {

        /*
            Initialise SDL
        */

        bool ok = SDL_Init(SDL_INIT_VIDEO) == 0;
        ACE_ASSERT(ok, "SDL Failed to initialise Video");

        /*
            Create a Window
            TODO: Parametrise Window dimensions and store somewhere
        */

        m_Window = SDL_CreateWindow(
            NULL,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            1280, 720,
            SDL_WINDOW_BORDERLESS
        );

        ACE_ASSERT(m_Window != NULL, "SDL Failed to open a Window");

        /*
            Create an SDL Renderer
        */

        m_Renderer = SDL_CreateRenderer(
            m_Window,
            -1,
            0
        );

        ACE_ASSERT(m_Renderer != NULL, "SDL Failed to create a Renderer");

        /*
            Create the PixelBuffer that the app will draw to.
        */

        m_PixelBuffer = new PixelBuffer(1280, 720);

        m_PixelBufferTexture = SDL_CreateTexture(
            m_Renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            1280, 720
        );

        Initialise();

        /*
            Main Loop
        */

        m_Running = true;
        m_MainClock.Start();

        while (m_Running) {
            Tick();
        }

        /*
            Cleanup
        */

        SDL_DestroyTexture(m_PixelBufferTexture);
        delete m_PixelBuffer;

        SDL_DestroyRenderer(m_Renderer);
        SDL_DestroyWindow(m_Window);
        SDL_Quit();
    }
    
    void App::Tick() {
        f64 dt = m_MainClock.Tick();

        // Event Handling
        {
            SDL_Event e;
            while (SDL_PollEvent(&e)) {
                switch (e.type) {
                    case SDL_QUIT:
                        m_Running = false;
                        break;
                    case SDL_KEYDOWN:
                        if (e.key.keysym.sym == SDLK_ESCAPE) {
                            m_Running = false;
                            break;
                        }
                    
                }
            }
        }

        // Update
        {
            Update(dt);
        }

        
        // Render
        {
            SDL_SetRenderDrawColor(m_Renderer, 255, 0, 255, 255);
            SDL_RenderClear(m_Renderer);

            Render(*m_PixelBuffer);

            SDL_UpdateTexture(
                m_PixelBufferTexture,
                NULL,
                m_PixelBuffer->Data,
                1280 * sizeof(u32)
            );

            SDL_RenderCopy(
                m_Renderer,
                m_PixelBufferTexture,
                NULL,
                NULL
            );

            SDL_RenderPresent(m_Renderer);
        }
    }
};