#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <stdio.h>
#include "gindex/gindex.h"
#include "gamestate/gamestate.h"
#include "components/collision/collision.h"
#include "components/physics/physics.h"

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

enum component_indexes {
	POSITION_INDEX,
	DRAW_INDEX
};

SDL_Texture *load_texture(const char *path);

SDL_Texture *gKeyPressSurfaces[KEY_PRESS_SURFACE_TOTAL];

SDL_Texture *gCurrentSurface = NULL;

int updatePositionSystem(struct gamestate *gs, struct gindex entity, int posindex) {
	struct position **positions = gs->components[posindex];
	if (!positions[entity.index] || positions[entity.index]->gen != entity.gen)
		return 1;
	positions[entity.index]->x++;
}
 
int DrawSystem(struct gamestate *gs, struct gindex entity, int posindex, int drawindex) {
	struct position **positions = gs->components[posindex];
	struct draw **draws = gs->components[drawindex];
	if (!positions[entity.index] || positions[entity.index]->gen != entity.gen)
		return 1;
	if (!draws[entity.index] || positions[entity.index]->gen != entity.gen)
		return 1;
	SDL_Rect DestR;
	DestR.x = positions[entity.index]->x;
	DestR.y = positions[entity.index]->y;
	int w, h;
	SDL_QueryTexture(draws[entity.index]->texture, NULL, NULL, &w, &h);
	DestR.w = w;
	DestR.h = h;

	SDL_RenderCopy(gRenderer, draws[entity.index]->texture, NULL,
	               &DestR);
} 

int physics_system(struct gamestate *gs, struct gindex entity, int phys_index, int pos_index, int col_index)
{
	struct physics **physicss = gs->components[phys_index];
	struct position **positions = gs->components[pos_index];
	struct collision **collisions = gs->components[col_index];
	if (!positions[entity.index] || positions[entity.index]->gen != entity.gen)
		return 1;
	if (!physicss[entity.index] || physicss[entity.index]->gen != entity.gen)
		return 1;
	if (!collisions[entity.index] || collisions[entity.index]-> gen != entity.gen)
		return 1;


	for (int i = 0; i < gs->allocator.num_entries; i++) {
		if (!collisions[i] || collisions[i]->gen != gs->entities[i].gen)
			continue;
		if (entity.index != i) {
			SDL_Rect nextbox = collisions[entity.index]->box;
			nextbox.x += physicss[entity.index]->velocity_x;
			nextbox.y += physicss[entity.index]->velocity_y;
			if (check_collision(nextbox, collisions[i]->box)) {
				//physicss[entity.index]->velocity_y = 0;
				return 1;
			}
		}
	}

	if (entity.index == 1)
		physicss[entity.index]->velocity_y += 0.0001;

	positions[entity.index]->x += physicss[entity.index]->velocity_x;
	positions[entity.index]->y += physicss[entity.index]->velocity_y;
	if (collisions[entity.index] && collisions[entity.index]->gen == entity.gen) {
		collisions[entity.index]->box.x = positions[entity.index]->x + collisions[entity.index]->offset_x;
		collisions[entity.index]->box.y = positions[entity.index]->y + collisions[entity.index]->offset_y;
	}
}

int main(int argc, char *args[])
{
	struct gamestate gs = init_gamestate();
	const int POSITION_INDEX = register_component(&gs);
	const int DRAW_INDEX = register_component(&gs);
	const int COL_INDEX = register_component(&gs);
	const int PHYS_INDEX = register_component(&gs);
	if (!init())
		printf("Failed to initialize!\n");
	else {
		struct gindex image = create_entity(&gs);
		struct gindex red_square = create_entity(&gs);
		struct gindex blue_square = create_entity(&gs);
		add_position(&gs, blue_square, 50, 400);
		add_draw(&gs, blue_square, load_texture("blue_square.png"));
		add_collision_box(&gs, blue_square, 25, 25, 50, 400, COL_INDEX);
		add_position(&gs, red_square, 50, 50);
		add_draw(&gs, red_square, load_texture("red_square.png"));
		add_collision_box(&gs, red_square, 25, 25, 0, 0, COL_INDEX);
		add_physics(&gs, red_square, 0, 0, PHYS_INDEX);
		add_position(&gs, image, 1, 1);
		add_draw(&gs, image, load_texture("press.bmp"));
		if (!load_media())
			printf("Failed to load media!\n");
		else {
			bool quit = false;
			SDL_Event e;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = true;
					else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
						case SDLK_UP:
							((struct physics**) gs.components[PHYS_INDEX])[1]->velocity_y = -0.25;
							break;
						}
					}
				}
				SDL_RenderClear(gRenderer);
				for (int i = 0; i < gs.allocator.num_entries; i++) {
					if (is_live(gs.allocator, gs.entities[i])) {
						//updatePositionSystem(&gs, gs.entities[i], POSITION_INDEX);
						DrawSystem(&gs, gs.entities[i], POSITION_INDEX, DRAW_INDEX);
						physics_system(&gs, gs.entities[i], PHYS_INDEX, POSITION_INDEX, COL_INDEX);
					}
				}
				SDL_RenderPresent(gRenderer);
			}
		}
	}

	//freeEntity(&image);
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
