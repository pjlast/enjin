#ifndef PHYSICS_H
#define PHYSICS_H

#include "../../gamestate/gamestate.h"
#include "../../gindex/gindex.h"

struct physics {
	int gen;
	float velocity_x;
	float velocity_y;
};

void add_physics(struct gamestate *gs, struct gindex e, float velocity_x,
                 float velocity_y, int index);
void clear_physics(struct gamestate *gs, struct gindex e, int index);

#endif
