#ifndef COLLISION_H
#define COLLISION_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "../../gamestate/gamestate.h"
#include "../../gindex/gindex.h"

struct collision {
	SDL_Rect box;
	float offset_x;
	float offset_y;
};

void add_collision_box(struct gamestate *gs, struct gindex e, float w,
                       float h, float x, float y, int index);
bool check_collision(SDL_Rect a, SDL_Rect b);

#endif
