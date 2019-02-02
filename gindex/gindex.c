#include "gindex.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

struct gindex galloc(struct gindex_allocator *allocator)
{
	if (allocator->num_free > 0) {
		uint64_t index = allocator->free[--allocator->num_free];

		allocator->entries[index].gen++;
		allocator->entries[index].is_live = true;

		struct gindex gi;
		gi.index = index;
		gi.gen = allocator->entries[index].gen;

		if (allocator->num_free == 0)
			free(allocator->free);
		else
			allocator->free = realloc(allocator->free,
				sizeof(uint64_t)*(allocator->num_free));

		return gi;
	}

	if (allocator->num_entries == 0)
		allocator->entries = malloc(sizeof(struct gindex_allocator_entry) * (allocator->num_entries + 1));
	else
		allocator->entries = realloc(allocator->entries,
		                             sizeof(struct gindex_allocator_entry) * (allocator->num_entries + 1));

	allocator->num_entries++;

	allocator->entries[allocator->num_entries - 1].is_live = true;
	allocator->entries[allocator->num_entries - 1].gen = 0;

	struct gindex gi;
	gi.index = allocator->num_entries - 1;
	gi.gen = 0;

	return gi;
}

bool gfree(struct gindex_allocator *allocator,
           struct gindex gi)
{
	if (gi.index > allocator->num_entries)
		return false;

	allocator->entries[gi.index].is_live = false;

	if (allocator->num_free == 0)
		allocator->free = malloc(sizeof(uint64_t));
	else
		allocator->free = realloc(allocator->free,
		                          sizeof(uint64_t)*(allocator->num_free + 1));

	allocator->num_free++;

	allocator->free[allocator->num_free - 1] = gi.index;
	return true;
}

bool is_live(struct gindex_allocator allocator,
             struct gindex gi)
{
	if (gi.index > allocator.num_entries
	    || gi.gen != allocator.entries[gi.index].gen)
		return false;

	return allocator.entries[gi.index].is_live;
}
