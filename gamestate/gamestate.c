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
	gs.clear_functions = NULL;
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
					sizeof(void*)*gs->allocator.num_entries);
	}
	gs->entities[e.index] = e;

	for (int i = 0; i < gs->num_components; i++) {
		(*(gs->clear_functions[i]))(gs, e, i);
	}
	return e;
}

void destroy_entity(struct gamestate *gs, struct gindex e)
{
	gfree(&gs->allocator, e);
}

int register_component(struct gamestate *gs, clearfunc clear)
{
	gs->num_components++;
	gs->components = realloc(gs->components,
	                         sizeof(void *)*gs->num_components);
	gs->components[gs->num_components - 1] = malloc(sizeof(void*));

	gs->clear_functions = realloc(gs->clear_functions,
	                              sizeof(clearfunc)*gs->num_components);
	gs->clear_functions[gs->num_components - 1] = clear;

	return (gs->num_components - 1);
}

void add_position(struct gamestate *gs, struct gindex e, float x, float y)
{
	gs->components[0] = realloc(gs->components[0], sizeof(struct position**)*(gs->allocator.num_entries));
	((struct position**) gs->components[0])[e.index] = malloc(sizeof(struct position));
	*(((struct position**) gs->components[0])[e.index]) = (struct position) {e.gen, x, y};
}

void add_draw(struct gamestate *gs, struct gindex e, SDL_Texture *texture)
{
	gs->components[1] = realloc(gs->components[1], sizeof(struct draw**)*(gs->allocator.num_entries));
	((struct draw**) gs->components[1])[e.index] = malloc(sizeof(struct draw));
	*(((struct draw**) gs->components[1])[e.index]) = (struct draw) {e.gen, texture};
}

void clear_draw(struct gamestate *gs, struct gindex e, int index)
{
	((struct draw**) gs->components[index])[e.index] = NULL;
}

void clear_position(struct gamestate *gs, struct gindex e, int index)
{
	((struct position**) gs->components[index])[e.index] = NULL;
}
