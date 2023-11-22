#include "core/app.hpp"

#include "SDL.h"

// TODO: Platform abstractions.

namespace Ace {
    void App::Run() {
        bool ok = SDL_Init(SDL_INIT_VIDEO) == 0;
        ACE_ASSERT(ok, "SDL Failed to initialise Video");

        m_Window = SDL_CreateWindow(
            NULL,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800, 600,
            SDL_WINDOW_BORDERLESS
        );

        ACE_ASSERT(m_Window != NULL, "SDL Failed to open a Window");

        m_Renderer = SDL_CreateRenderer(
            m_Window,
            -1,
            0
        );

        ACE_ASSERT(m_Renderer != NULL, "SDL Failed to create a Renderer");

        m_Running = true;
        while (m_Running) {
            Tick();
        }
    }
    
    void App::Tick() {
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

        SDL_SetRenderDrawColor(m_Renderer, 255, 0, 0, 255);
        SDL_RenderClear(m_Renderer);
        SDL_RenderPresent(m_Renderer);

        Update(0.0);
        Render();
    }
};