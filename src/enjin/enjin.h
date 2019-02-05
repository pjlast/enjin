#ifndef ENJIN_H
#define ENJIN_H

#include <SDL2/SDL.h>
#include <stdbool.h>

bool enjin_initialize(SDL_Window **window, SDL_Renderer **renderer);
void enjin_cleanup(SDL_Window **window, SDL_Renderer **renderer);

#endif
