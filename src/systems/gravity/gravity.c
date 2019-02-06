#include "gravity.h"
#include "../../components/physics/physics.h"

void gravity(struct gamestate *gs, struct gindex e, int physindex)
{
	struct physics **physcomps = (struct physics**) gs->comps[physindex];
	if (!physcomps[e.index] || !gravcomps[e.index])
		return;

	physics[e.index]->velocity_y += 980*1/60.0;
}
