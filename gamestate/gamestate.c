#include "gamestate.h"
#include "../gindex/gindex.h"
#include <stdlib.h>
#include <SDL2/SDL.h>

struct gamestate init_gamestate(void)
{
	struct gamestate gs;
	gs.entity_allocator.num_entries = 0;
	gs.entity_allocator.num_free = 0;
	gs.entities = malloc(sizeof(struct gindex));
	gs.positions = malloc(sizeof(struct position*));
	gs.draws = malloc(sizeof(struct draw*));
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
	gs->positions[entity.index] = malloc(sizeof(struct position));
	*gs->positions[entity.index] = (struct position) {x, y};
}

void add_draw(struct gamestate *gs, struct gindex entity, SDL_Texture *texture)
{
	gs->draws[entity.index] = malloc(sizeof(struct draw));
	gs->draws[entity.index]->texture = texture;
}
