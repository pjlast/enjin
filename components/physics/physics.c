#include "physics.h"
#include <stdio.h>

void add_physics(struct gamestate *gs, struct gindex e, float velocity_x,
                 float velocity_y, int index)
{
        gs->components[index] = realloc(gs->components[index],
                                        sizeof(struct physics**)*(gs->allocator.num_entries));
        ((struct physics**) gs->components[index])[e.index] = malloc(sizeof(struct physics));
        *(((struct physics**) gs->components[index])[e.index]) = (struct physics) {e.gen, velocity_x, velocity_y};
}
