#include "enjin/enjin.h"
#include "gamestate/gamestate.h"
#include "components/collision/collision.h"
#include "components/physics/physics.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

bool load_media(const char **files, SDL_Texture **textures, int num_files,
                SDL_Renderer *renderer);
SDL_Texture *load_texture(const char *path, SDL_Renderer *renderer);

int draw_system(struct gamestate *gs, struct gindex e, int posindex, int drawindex, SDL_Renderer *renderer) {
	struct position **positions = (struct position**) gs->comps[posindex];
	struct draw **draws = (struct draw**) gs->comps[drawindex];
	if (!positions[e.index])
		return 1;
	if (!draws[e.index])
		return 1;
	SDL_Rect DestR;
	DestR.x = positions[e.index]->x;
	DestR.y = positions[e.index]->y;
	int w, h;
	SDL_QueryTexture(draws[e.index]->texture, NULL, NULL, &w, &h);
	DestR.w = w;
	DestR.h = h;

	SDL_RenderCopy(renderer, draws[e.index]->texture, NULL,
	               &DestR);
	return 0;
} 

int physics_system(struct gamestate *gs, struct gindex e, int phys_index, int pos_index, int col_index)
{
	struct physics **physicss = (struct physics**) gs->comps[phys_index];
	struct position **positions = (struct position**) gs->comps[pos_index];
	struct collision **collisions = (struct collision**) gs->comps[col_index];
	if (!positions[e.index])
		return 1;
	if (!physicss[e.index])
		return 1;
	if (!collisions[e.index])
		return 1;


	for (uint64_t i = 0; i < gs->alloc.numents; i++) {
		if (!collisions[i])
			continue;
		if (e.index != i) {
			SDL_Rect nextbox = collisions[e.index]->box;
			nextbox.x += physicss[e.index]->velocity_x*1/60.0;
			nextbox.y += physicss[e.index]->velocity_y*1/60.0;
			if (check_collision(nextbox, collisions[i]->box)) {
				//physicss[e.index]->velocity_y = 0;
				return 1;
			}
		}
	}

	if (e.index == 1)
		physicss[e.index]->velocity_y += 980*1/60.0;

	positions[e.index]->x += physicss[e.index]->velocity_x*1/60.0;
	positions[e.index]->y += physicss[e.index]->velocity_y*1/60.0;
	if (collisions[e.index]) {
		collisions[e.index]->box.x = positions[e.index]->x + collisions[e.index]->offset_x;
		collisions[e.index]->box.y = positions[e.index]->y + collisions[e.index]->offset_y;
	}
	return 0;
}

int main()
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	if (!enjin_initialize(&window, &renderer)) {
		fprintf(stderr, "Failed to initialize enjin!\n");
		return 1;
	}

	// Texture init logic and such here
	const char *files[] = {"press.bmp",
	                       "blue_square.png",
	                       "red_square.png"};

	SDL_Texture *textures[3];

	if (!load_media(files, textures, 3, renderer)) {
		fprintf(stderr, "Failed to load media!\n");
		enjin_cleanup(&window, &renderer);
		return 1;
	}
	// TODO: Add texture cleanup logic
	
	// Entity creation and such here
	struct gamestate gs = init_gamestate();
	const int POSITION_INDEX = register_component(&gs);
	const int DRAW_INDEX = register_component(&gs);
	const int COL_INDEX = register_component(&gs);
	const int PHYS_INDEX = register_component(&gs);

	struct gindex image = create_entity(&gs);
	struct gindex red_square = create_entity(&gs);
	struct gindex blue_square = create_entity(&gs);
	add_position(&gs, blue_square, 50, 400);
	add_draw(&gs, blue_square, textures[1]);
	add_collision_box(&gs, blue_square, 25, 25, 50, 400, COL_INDEX);
	add_position(&gs, red_square, 50, 50);
	add_draw(&gs, red_square, textures[2]);
	add_collision_box(&gs, red_square, 25, 25, 0, 0, COL_INDEX);
	add_physics(&gs, red_square, 0, 0, PHYS_INDEX);
	add_position(&gs, image, 1, 1);
	add_draw(&gs, image, textures[0]);
	struct gindex blue_square2 = create_entity(&gs);
	add_position(&gs, blue_square2, 200, 400);
	add_draw(&gs, blue_square2, textures[1]);
	add_collision_box(&gs, blue_square2, 25, 25, 200, 400, COL_INDEX);

	// Main event loop
	bool quit = false;
	SDL_Event e;
	while (!quit) {
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				quit = true;
			} else if (e.type == SDL_KEYDOWN) {
				switch (e.key.keysym.sym) {
				case SDLK_UP:
					((struct physics**) gs.comps[PHYS_INDEX])[1]->velocity_y = -700;
					break;
				case SDLK_RIGHT:
					((struct physics**) gs.comps[PHYS_INDEX])[1]->velocity_x = 300;
					break;
				case SDLK_LEFT:
					((struct physics**) gs.comps[PHYS_INDEX])[1]->velocity_x = -300;
					break;
				}
			}
			else if (e.type == SDL_KEYUP) {
				switch (e.key.keysym.sym) {
				case SDLK_RIGHT:
				case SDLK_LEFT:
					((struct physics**) gs.comps[PHYS_INDEX])[1]->velocity_x = 0;
					break;
				}
			}
		}

		SDL_RenderClear(renderer);
		// Systems logic here
		for (uint64_t i = 0; i < gs.alloc.numents; i++) {
			if (islive(gs.alloc, gs.ents[i])) {
				//updatePositionSystem(&gs, gs.ents[i], POSITION_INDEX);
				draw_system(&gs, gs.ents[i], POSITION_INDEX, DRAW_INDEX, renderer);
				physics_system(&gs, gs.ents[i],PHYS_INDEX, POSITION_INDEX, COL_INDEX);
			}
		}
		SDL_RenderPresent(renderer);
	}

	enjin_cleanup(&window, &renderer);
}

bool load_media(const char **files, SDL_Texture **textures, int num_files,
                SDL_Renderer *renderer)
{
	for (int i = 0; i < num_files; i++) {
		textures[i] = load_texture(files[i], renderer);
		
		if (textures[i] == NULL) {
			for (int j = 0; j < i; j++)
				SDL_DestroyTexture(textures[j]);

			fprintf(stderr, "Failed to load textures!\n");
			return false;
		}
	}

	return true;
}

SDL_Texture *load_texture(const char *path, SDL_Renderer *renderer)
{
	SDL_Texture *texture = NULL;
	SDL_Surface *loaded_surface = IMG_Load(path);

	if (loaded_surface == NULL) {
		fprintf(stderr, "Unable to load image %s! SDL_image Error: "
		                "%s\n", path, IMG_GetError());
		return NULL;
	}

	texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);

	if (texture == NULL)
		fprintf(stderr, "Unable to create texture from %s! SDL_Error: "
		                "%s\n", path, SDL_GetError());

	SDL_FreeSurface(loaded_surface);

	return texture;
}
