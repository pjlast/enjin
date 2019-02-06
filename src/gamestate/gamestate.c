#include "gamestate.h"
#include "../gindex/gindex.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdio.h>

struct gamestate init_gamestate(void)
{
	struct gamestate gs;
	gs.alloc = init_gindex_allocator();
	gs.numcomps = 0;
	gs.ents = NULL;
	gs.comps = NULL;
	return gs;
}

struct gindex create_entity(struct gamestate *gs)
{
	struct gindex e = galloc(&(gs->alloc));
	if (e.gen == 0) {
		gs->ents = realloc(gs->ents,
		                   sizeof(struct gindex)*gs->alloc.numents);
		for (int i = 0; i < gs->numcomps; i++) {
			gs->comps[i] = realloc(gs->comps[i],
					sizeof(void*)*gs->alloc.numents);
			gs->comps[i][e.index] = NULL;
		}
	}
	gs->ents[e.index] = e;

	return e;
}

void destroy_entity(struct gamestate *gs, struct gindex e)
{
	gfree(&gs->alloc, e);
	for (int i = 0; i < gs->numcomps; i++)
		gs->comps[i][e.index] = NULL;
	
}

int register_component(struct gamestate *gs)
{
	gs->numcomps++;
	gs->comps = realloc(gs->comps, sizeof(void *)*gs->numcomps);
	gs->comps[gs->numcomps - 1] = malloc(sizeof(void*));

	return (gs->numcomps - 1);
}

void add_position(struct gamestate *gs, struct gindex e, float x, float y)
{
	gs->comps[0] = realloc(gs->comps[0], sizeof(struct position**)*(gs->alloc.numents));
	((struct position**) gs->comps[0])[e.index] = malloc(sizeof(struct position));
	*(((struct position**) gs->comps[0])[e.index]) = (struct position) {x, y};
}

void add_draw(struct gamestate *gs, struct gindex e, SDL_Texture *texture)
{
	gs->comps[1] = realloc(gs->comps[1], sizeof(struct draw**)*(gs->alloc.numents));
	((struct draw**) gs->comps[1])[e.index] = malloc(sizeof(struct draw));
	*(((struct draw**) gs->comps[1])[e.index]) = (struct draw) {texture};
}
