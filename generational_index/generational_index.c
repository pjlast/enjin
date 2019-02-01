#include "generational_index.h"
#include <stdbool.h>
#include <stdlib.h>

generational_index_t allocate(generational_index_allocator_t *allocator)
{
	if (allocator->num_free > 0) {
		int index = allocator->free[allocator->num_free - 1];

		allocator->entries[index].generation++;
		allocator->entries[index].is_live = true;

		generational_index_t gi;
		gi.index = index;
		gi.generation = allocator->entries[index].generation;

		allocator->free = realloc(allocator->free,
			sizeof(unsigned int)*allocator->num_free--);

		return gi;
	}

	if (allocator->num_entries == 0)
		allocator->entries = malloc(sizeof(allocator_entry_t));
	else
		allocator->entries = realloc(allocator->entries,
			sizeof(allocator_entry_t)*(allocator->num_entries));
	allocator->num_entries++;

	allocator->entries[allocator->num_entries - 1].is_live = true;
	allocator->entries[allocator->num_entries].generation = 0;

	generational_index_t gi;
	gi.index = allocator->num_entries - 1;
	gi.generation = 0;

	return gi;
}

bool deallocate(generational_index_allocator_t *allocator,
                generational_index_t gi)
{
	if (gi.index > allocator->num_entries)
		return false;

	allocator->entries[gi.index].is_live = false;
	if (allocator->num_free == 0)
		allocator->free = malloc(sizeof(unsigned int));
	else
		allocator->free = realloc(allocator->free,
		                          sizeof(unsigned int)*(allocator->num_free + 1));

	allocator->free[allocator->num_free++] = gi.index;
	return true;
}

bool is_live(generational_index_allocator_t allocator,
             generational_index_t gi)
{
	if (gi.index > allocator.num_entries)
		return false;

	if (gi.generation != allocator.entries[gi.index].generation)
		return false;

	return allocator.entries[gi.index].is_live;
}
