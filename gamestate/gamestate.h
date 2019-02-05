#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <SDL2/SDL.h>
#include "../gindex/gindex.h"


struct position {
	int gen;
	float x;
	float y;
};

struct draw {
	int gen;
	SDL_Texture *texture;
};

struct gamestate;

typedef void (*clearfunc)(struct gamestate*, struct gindex, int);

struct gamestate {
	struct gindex_allocator allocator;
	struct gindex *entities;
	void **components;
	clearfunc *clear_functions;
	int num_components;
};

struct gamestate init_gamestate(void);
struct gindex create_entity(struct gamestate *gs);
void destroy_entity(struct gamestate *gs, struct gindex e);
int register_component(struct gamestate *gs,
		clearfunc);
void add_position(struct gamestate *gs, struct gindex e, float x, float y);
void add_draw(struct gamestate *gs, struct gindex e, SDL_Texture *texture);
void clear_draw(struct gamestate *gs, struct gindex e, int index);
void clear_position(struct gamestate *gs, struct gindex e, int index);

#endif
