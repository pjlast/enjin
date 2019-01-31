#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <stdbool.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool load_media();
void cleanup();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

int main(int argc, char *args[])
{
	if (!init())
		printf("Failed to initialize!\n");
	else {
		if (!load_media())
			printf("Failed to load media!\n");
		else {
			bool quit = false;
			SDL_Event e;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = true;
				}
				SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, NULL);
				SDL_UpdateWindowSurface(gWindow);
			}
		}
	}

	cleanup();
	return 0;
}

bool init()
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n",
		       SDL_GetError());
		success = false;
	} else {
		gWindow = SDL_CreateWindow("SDL Tutorial",
		                          SDL_WINDOWPOS_UNDEFINED,
		                          SDL_WINDOWPOS_UNDEFINED,
		                          SCREEN_WIDTH,
		                          SCREEN_HEIGHT,
		                          SDL_WINDOW_SHOWN );
		if (gWindow == NULL) {
			printf("Window could not be created! SDL_Error: %s\n",
			       SDL_GetError());
			success = false;
		} else {
			gScreenSurface = SDL_GetWindowSurface(gWindow);
		}
	}
	return success;
}

bool load_media()
{
	bool success = true;

	gHelloWorld = SDL_LoadBMP("hello.bmp");
	if (gHelloWorld == NULL) {
		printf("Unable to load image %s! SDL Error: %s\n", "hello.bmp");
		success = false;
	}

	return success;
}

void cleanup()
{
	SDL_FreeSurface(gHelloWorld);
	gHelloWorld = NULL;

	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	SDL_Quit();
}
