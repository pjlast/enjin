#include "enjin.h"
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

bool enjin_initialize(SDL_Window **window, SDL_Renderer **renderer)
{
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n",
		        SDL_GetError());
		return false;
	}

	*window = SDL_CreateWindow("SDL Tutorial",
				  SDL_WINDOWPOS_UNDEFINED,
				  SDL_WINDOWPOS_UNDEFINED,
				  SCREEN_WIDTH,
				  SCREEN_HEIGHT,
				  SDL_WINDOW_SHOWN);
	if (*window == NULL) {
		fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
		        SDL_GetError());
		return false;
	}

	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED
	                                          | SDL_RENDERER_PRESENTVSYNC);
	if (*renderer == NULL) {
		fprintf(stderr, "Renderer could not be created! SDL_Error: \
				%s\n", SDL_GetError());
		SDL_DestroyWindow(*window);
		return false;
	}

	SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	int img_flags = IMG_INIT_PNG;
	if (!(IMG_Init(img_flags) & img_flags)) {
		fprintf(stderr, "SDL_image could not initialize! SDL_image \
				Error: %s\n", IMG_GetError());
		SDL_DestroyRenderer(*renderer);
		SDL_DestroyWindow(*window);
		return false;
	}

	return true;
}

void enjin_cleanup(SDL_Window **window, SDL_Renderer **renderer)
{
	SDL_DestroyRenderer(*renderer);
	SDL_DestroyWindow(*window);
	*window = NULL;
	*renderer = NULL;

	IMG_Quit();
	SDL_Quit();
}
