#ifndef GINDEX_H
#define GINDEX_H

#include <stdbool.h>
#include <stdint.h>

struct gindex {
	uint64_t index;
	uint64_t gen;
};

struct gindex_allocator {
	struct gindex_allocator_entry *ents;
	uint64_t numents;
	uint64_t *free;
	uint64_t numfree;
};

struct gindex_allocator_entry {
	bool islive;
	uint64_t gen;
};

struct gindex_allocator init_gindex_allocator(void);
struct gindex galloc(struct gindex_allocator *alloc);
bool gfree(struct gindex_allocator *alloc,
           struct gindex gi);
bool islive(struct gindex_allocator alloc,
             struct gindex gi);

#endif
