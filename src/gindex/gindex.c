#include "gindex.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

struct gindex_allocator init_gindex_allocator(void)
{
	struct gindex_allocator alloc = {NULL, 0, NULL, 0};
	
	return alloc;
}

struct gindex galloc(struct gindex_allocator *alloc)
{
	if (alloc->numfree > 0) {
		uint64_t index = alloc->free[--alloc->numfree];

		alloc->ents[index].gen++;
		alloc->ents[index].islive = true;

		struct gindex gi = {index, alloc->ents[index].gen};

		alloc->free = realloc(alloc->free,
		                      sizeof(uint64_t)*(alloc->numfree));

		return gi;
	}

	alloc->ents = realloc(alloc->ents,
	                      sizeof(struct gindex_allocator_entry)
	                      *(++alloc->numents));

	uint64_t index = alloc->numents - 1;
	alloc->ents[index] = (struct gindex_allocator_entry) {true, 0};

	struct gindex gi = {index, 0};

	return gi;
}

bool gfree(struct gindex_allocator *alloc,
           struct gindex gi)
{
	if (gi.index > alloc->numents)
		return false;

	alloc->ents[gi.index].islive = false;

	alloc->free = realloc(alloc->free,
	                      sizeof(uint64_t)*(++alloc->numfree));

	alloc->free[alloc->numfree - 1] = gi.index;
	return true;
}

bool islive(struct gindex_allocator alloc,
            struct gindex gi)
{
	if (gi.index > alloc.numents
	    || gi.gen != alloc.ents[gi.index].gen)
		return false;

	return alloc.ents[gi.index].islive;
}
