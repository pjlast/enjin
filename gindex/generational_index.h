#ifndef GENERATIONAL_INDEX_H
#define GENERATIONAL_INDEX_H

#include <stdbool.h>
#include <stdint.h>

struct gindex {
	uint64_t index;
	uint64_t gen;
};

struct gindex_allocator {
	struct gindex_allocator_entry *entries;
	uint64_t num_entries;
	uint64_t *free;
	uint64_t num_free;
};

struct gindex_allocator_entry {
	bool is_live;
	uint64_t gen;
};

struct gindex galloc(struct gindex_allocator *allocator);
bool gfree(struct gindex_allocator *allocator,
           struct gindex gi);
bool is_live(struct gindex_allocator allocator,
             struct gindex gi);

#endif
