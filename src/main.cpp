#include <stdio.h>

#include "SDL.h"

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);
    printf("Hello world!");
    SDL_Quit();
}