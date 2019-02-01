#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int index;
	int generation;
} generational_index_t;

typedef struct {
	bool is_live;
	int generation;
} allocator_entry_t;

typedef struct {
	allocator_entry_t *entries;
	int *free;
} generational_index_allocator_t;

int main(int argc, char *args[])
{
	generational_index_allocator_t giat;
	giat.free = malloc(sizeof(int));
	giat.free[0] = 1;
	giat.free = realloc(giat.free, sizeof(int) * 2);
	giat.free[1] = 2;

	printf("%d %d", giat.free[0], giat.free[1]);
	if (giat.free) {
	}
	return 0;
}
