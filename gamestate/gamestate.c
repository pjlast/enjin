#include "gamestate.h"
#include "../gindex/gindex.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdio.h>

struct gamestate init_gamestate(void)
{
	struct gamestate gs;
	gs.allocator.num_entries = 0;
	gs.allocator.num_free = 0;
	gs.allocator.free = NULL;
	gs.allocator.entries = NULL;
	gs.num_components = 0;
	gs.entities = NULL;
	gs.components = NULL;
	return gs;
}

struct gindex create_entity(struct gamestate *gs)
{
	struct gindex e = galloc(&(gs->allocator));
	if (e.gen == 0) {
		gs->entities = realloc(gs->entities,
			sizeof(struct gindex)*gs->allocator.num_entries);
		for (int i = 0; i < gs->num_components; i++) {
			gs->components[i] = realloc(gs->components[i],
					sizeof(void*)*gs->allocator.num_entries);
			gs->components[i][e.index] = NULL;
		}
	}
	gs->entities[e.index] = e;

	return e;
}

void destroy_entity(struct gamestate *gs, struct gindex e)
{
	gfree(&gs->allocator, e);
	for (int i = 0; i < gs->num_components; i++)
		gs->components[i][e.index] = NULL;
	
}

int register_component(struct gamestate *gs)
{
	gs->num_components++;
	gs->components = realloc(gs->components,
	                         sizeof(void *)*gs->num_components);
	gs->components[gs->num_components - 1] = malloc(sizeof(void*));

	return (gs->num_components - 1);
}

void add_position(struct gamestate *gs, struct gindex e, float x, float y)
{
	gs->components[0] = realloc(gs->components[0], sizeof(struct position**)*(gs->allocator.num_entries));
	((struct position**) gs->components[0])[e.index] = malloc(sizeof(struct position));
	*(((struct position**) gs->components[0])[e.index]) = (struct position) {x, y};
}

void add_draw(struct gamestate *gs, struct gindex e, SDL_Texture *texture)
{
	gs->components[1] = realloc(gs->components[1], sizeof(struct draw**)*(gs->allocator.num_entries));
	((struct draw**) gs->components[1])[e.index] = malloc(sizeof(struct draw));
	*(((struct draw**) gs->components[1])[e.index]) = (struct draw) {texture};
}
