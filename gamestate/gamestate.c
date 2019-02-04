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
		for (int i = 0; i < gs->num_components; i++)
			gs->components[i] = realloc(gs->components[i],
					gs->allocator.num_entries);
	}
	gs->entities[e.index] = e;
	return e;
}

void destroy_entity(struct gamestate *gs, struct gindex e)
{
	gfree(&gs->allocator, e);
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
	struct position ***positions = malloc(sizeof(struct position**));
	*positions = malloc(sizeof(struct position*));
	*positions[e.index] = malloc(sizeof(struct position));
	**positions[e.index] = (struct position) {e.gen, x, y};
	gs->components[0] = positions;
}

void add_draw(struct gamestate *gs, struct gindex e, SDL_Texture *texture)
{
	struct draw ***draws = malloc(sizeof(struct draw**));
	*draws = malloc(sizeof(struct draw*));
	*draws[e.index] = malloc(sizeof(struct draw));
	**draws[e.index] = (struct draw) {e.gen, texture};
	gs->components[1] = draws;
}
