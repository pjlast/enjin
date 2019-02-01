#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool load_media();
void cleanup();

SDL_Window *gWindow = NULL;

SDL_Renderer *gRenderer = NULL;

SDL_Texture *gTexture = NULL;

enum key_press_surfaces {
	KEY_PRESS_SURFACE_DEFAULT,
	KEY_PRESS_SURFACE_UP,
	KEY_PRESS_SURFACE_DOWN,
	KEY_PRESS_SURFACE_LEFT,
	KEY_PRESS_SURFACE_RIGHT,
	KEY_PRESS_SURFACE_TOTAL
};

SDL_Texture *load_texture(const char *path);

SDL_Texture *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

SDL_Texture *gCurrentSurface = NULL;

typedef struct {
	generational_index_t *entities;
	int num_entities;
} game_state;

typedef struct {
	int index,
	int generation
} generational_index_t;

typedef struct {
	bool is_live,
	int generation
} allocator_entry_t;

typedef struct {
	allocator_entry_t *entries,
	int *free;
	int num_free;
} generational_index_allocator_t;

generational_index_t allocate(generational_index_allocator_t *allocator) {
	generational_index_t gi;
	gi.index = 0;
	gi.generation = 0;

	return gi;
}

bool deallocate(generational_index_allocator_t *allocator,
                generational_index_t *index) {

}

bool is_live(generational_index_allocator_t allocator,
             generational_index_t index) {
	
}

typedef struct {
	float x;
	float y;
} position_t;

typedef struct {
	SDL_Texture *texture;
} draw_t;

typedef struct {
	position_t *position;
	draw_t *draw;
} components_t;

typedef struct {
	unsigned int uuid;
	components_t components;
} entity_t;

void addComponentPosition(entity_t *entity, int x, int y) {
	entity->components.position = malloc(sizeof(component_position));
	entity->components.position->x = x;
	entity->components.position->y = y;
}
 
void addComponentDraw(entity *e, SDL_Texture *texture) {
	e->components.draw = malloc(sizeof(component_draw));
	e->components.draw->texture = texture;
}

void freeEntity(entity *e) {
	if (e->components.position)
		free(e->components.position);
	if (e->components.draw)
		free(e->components.draw);
}

int updatePositionSystem(entity *e) {
if (!e->components.position) return 1;
	e->components.position->x++;
}
 
int DrawSystem(entity *e) {
if (!e -> components.position || ! e -> components.draw) return 1;
	SDL_RenderClear(gRenderer);
	SDL_Rect DestR;
	DestR.x = e->components.position->x;
	DestR.y = e->components.position->y;
	DestR.w = 640;
	DestR.h = 480;

	SDL_RenderCopy(gRenderer, e->components.draw->texture, NULL, &DestR);
	SDL_RenderPresent(gRenderer);
} 

int main(int argc, char *args[])
{
	entity image;
	addComponentPosition(&image, 0, 0);
	addComponentDraw(&image, gCurrentSurface);
	if (!init())
		printf("Failed to initialize!\n");
	else {
		if (!load_media())
			printf("Failed to load media!\n");
		else {
			bool quit = false;
			SDL_Event e;
			image.components.draw->texture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = true;
					else if (e.type == SDL_KEYDOWN) {
						switch(e.key.keysym.sym) {
						case SDLK_UP:
							image.components.draw->texture = gKeyPressSurfaces[KEY_PRESS_SURFACE_UP];
							break;
						case SDLK_DOWN:
							image.components.draw->texture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN];
							break;
						case SDLK_LEFT:
							image.components.draw->texture = gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT];
							break;
						case SDLK_RIGHT:
							image.components.draw->texture = gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT];
							break;
						default:
							image.components.draw->texture = gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT];
						}
					}
				}
				DrawSystem(&image);
			}
		}
	}

	freeEntity(&image);
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
			gRenderer = SDL_CreateRenderer(gWindow, -1,
					SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n",
				       SDL_GetError());
				success = false;
			} else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF,
				                       0xFF, 0xFF);
				int img_flags = IMG_INIT_PNG;
				if (!(IMG_Init(img_flags) & img_flags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n",
					       IMG_GetError());
					success = false;
				}
			}
		}
	}
	return success;
}

bool load_media()
{
	bool success = true;

	gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] = load_texture("press.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DEFAULT] == NULL) {
		printf("Failed to load default image!\n");
		success = false;
	}

	gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] = load_texture("up.png");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_UP] == NULL) {
		printf("Failed to load up image!\n");
		success = false;
	}

	gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] = load_texture("down.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_DOWN] == NULL) {
		printf("Failed to load down image!\n");
		success = false;
	}

	gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] = load_texture("left.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_LEFT] == NULL) {
		printf("Failed to load left image!\n");
		success = false;
	}

	gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] = load_texture("right.bmp");
	if (gKeyPressSurfaces[KEY_PRESS_SURFACE_RIGHT] == NULL) {
		printf("Failed to load right image!\n");
		success = false;
	}

	return success;
}

void cleanup()
{
	for (int i = 0; i < KEY_PRESS_SURFACE_TOTAL; i++) {
		SDL_DestroyTexture(gKeyPressSurfaces[i]);
		gKeyPressSurfaces[i] = NULL;
	}

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture *load_texture(const char *path)
{
	SDL_Texture *new_texture = NULL;

	SDL_Surface *loaded_surface = IMG_Load(path);

	if (loaded_surface == NULL)
		printf("Unable to load image %s! SDL_image Error: %s\n",
		       path,
		       IMG_GetError());
	else {
		new_texture = SDL_CreateTextureFromSurface(gRenderer,
		                                           loaded_surface);
		if (new_texture == NULL)
			printf("Unable to create texture from %s! SDL Error: %s\n",
			       path,
			       SDL_GetError());
		SDL_FreeSurface(loaded_surface);
	}

	return new_texture;
}
