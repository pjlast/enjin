#include "gamestate.h"
#include "../gindex/gindex.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <stdio.h>

struct gamestate init_gamestate(void)
{
	struct gamestate gs;
	gs.entity_allocator.num_entries = 0;
	gs.entity_allocator.num_free = 0;
	gs.num_components = 0;
	gs.entities = malloc(sizeof(struct gindex));
	gs.components = malloc(sizeof(void *)*2);
	gs.components[0] = malloc(sizeof(struct position**));
	*((struct position**) gs.components[0]) = malloc(sizeof(struct position*));
	gs.components[1] = malloc(sizeof(struct draw***));
	return gs;
}

struct gindex create_entity(struct gamestate *gs)
{
	struct gindex entity = galloc(&(gs->entity_allocator));
	if (entity.gen == 0)
		gs->entities = realloc(gs->entities,
			sizeof(struct gindex)*gs->entity_allocator.num_entries);
	gs->entities[entity.index] = entity;
	return entity;
}

void add_position(struct gamestate *gs, struct gindex entity, float x, float y)
{
	struct position ***positions = malloc(sizeof(struct position**));
	*positions = malloc(sizeof(struct position*));
	*positions[entity.index] = malloc(sizeof(struct position));
	**positions[entity.index] = (struct position) {x, y};
	gs->components[0] = positions;
}

void add_draw(struct gamestate *gs, struct gindex entity, SDL_Texture *texture)
{
	struct draw ***draws = malloc(sizeof(struct draw**));
	*draws = malloc(sizeof(struct draw*));
	*draws[entity.index] = malloc(sizeof(struct draw));
	**draws[entity.index] = (struct draw) {texture};
	gs->components[1] = draws;
}
