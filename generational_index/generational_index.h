#ifndef GENERATIONAL_INDEX_H
#define GENERATIONAL_INDEX_H

#include <stdbool.h>

typedef struct generational_index {
	unsigned int index;
	unsigned int generation;
} generational_index_t;

typedef struct allocator_entry {
	bool is_live;
	unsigned int generation;
} allocator_entry_t;

typedef struct generational_index_allocator {
	allocator_entry_t *entries;
	unsigned int num_entries;
	unsigned int *free;
	unsigned int num_free;
} generational_index_allocator_t;

generational_index_t allocate(generational_index_allocator_t *allocator);
bool deallocate(generational_index_allocator_t *allocator,
                generational_index_t gi);
bool is_live(generational_index_allocator_t allocator,
             generational_index_t gi);

#endif
