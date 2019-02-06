#include "physics.h"
#include <stdio.h>

void add_physics(struct gamestate *gs, struct gindex e, float velocity_x,
                 float velocity_y, int index)
{
        gs->comps[index] = realloc(gs->comps[index],
                                        sizeof(struct physics**)*(gs->alloc.numents));
        ((struct physics**) gs->comps[index])[e.index] = malloc(sizeof(struct physics));
        *(((struct physics**) gs->comps[index])[e.index]) = (struct physics) {velocity_x, velocity_y};
}
