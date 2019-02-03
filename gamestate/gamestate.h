#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SDL2/SDL.h>
#include "../gindex/gindex.h"

struct position {
	float x;
	float y;
};

struct draw {
	SDL_Texture *texture;
};

struct gamestate {
	struct gindex_allocator entity_allocator;
	struct gindex *entities;
	struct position **positions;
	struct draw **draws;
};

struct gamestate init_gamestate(void);
struct gindex create_entity(struct gamestate *gs);
void add_position(struct gamestate *gs, struct gindex, float x, float y);
void add_draw(struct gamestate *gs, struct gindex entity, SDL_Texture *texture);

#endif
