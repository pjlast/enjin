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

struct gamestate;

struct gamestate {
	struct gindex_allocator alloc;
	struct gindex *ents;
	void ***comps;
	int numcomps;
};

struct gamestate init_gamestate(void);
struct gindex create_entity(struct gamestate *gs);
void destroy_entity(struct gamestate *gs, struct gindex e);
int register_component(struct gamestate *gs);
void add_position(struct gamestate *gs, struct gindex e, float x, float y);
void add_draw(struct gamestate *gs, struct gindex e, SDL_Texture *texture);

#endif
