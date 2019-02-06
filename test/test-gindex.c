#include "../unity/src/unity.h"
#include "../src/gindex/gindex.h"

struct gindex_allocator setup_alloc(void)
{
	struct gindex_allocator alloc;

	alloc.numfree = 0;
	alloc.numents = 0;
	alloc.free = NULL;
	alloc.ents = NULL;

	return alloc;
}

void test_galloc_should_return_generational_index(void)
{
	struct gindex_allocator alloc = setup_alloc();
	struct gindex gi = galloc(&alloc);
	TEST_ASSERT_EQUAL_INT(0, gi.index);
	TEST_ASSERT_EQUAL_INT(0, gi.gen);
}

void test_second_galloc_returns_incrementing_index(void)
{
	struct gindex_allocator alloc = setup_alloc();
	struct gindex gi1 = galloc(&alloc);
	struct gindex gi2 = galloc(&alloc);
	TEST_ASSERT_EQUAL_INT(0, gi1.index);
	TEST_ASSERT_EQUAL_INT(0, gi1.gen);
	TEST_ASSERT_EQUAL_INT(1, gi2.index);
	TEST_ASSERT_EQUAL_INT(0, gi2.gen);
}

void test_gfree_adds_free_index_to_pool(void)
{
	struct gindex_allocator alloc = setup_alloc();
	struct gindex gi1 = galloc(&alloc);
	bool did_free = gfree(&alloc, gi1);
	TEST_ASSERT(did_free);
	TEST_ASSERT_EQUAL_INT(1, alloc.numfree);
	TEST_ASSERT_EQUAL_INT(0, alloc.free[0]);
	TEST_ASSERT(!islive(alloc, gi1));
}

void test_galloc_gallocs_free_index_if_available(void)
{
	struct gindex_allocator alloc = setup_alloc();
	struct gindex gi1 = galloc(&alloc);
	gfree(&alloc, gi1);
	struct gindex gi2 = galloc(&alloc);
	TEST_ASSERT_EQUAL_INT(0, gi2.index);
	TEST_ASSERT_EQUAL_INT(1, gi2.gen);
	TEST_ASSERT_EQUAL_INT(0, alloc.numfree);
}

void test_islive_returns_true_if_index_in_use(void)
{
	struct gindex_allocator alloc = setup_alloc();
	struct gindex gi1 = galloc(&alloc);
	TEST_ASSERT(islive(alloc, gi1));
}

void test_islive_returns_false_if_index_gfreed(void)
{
	struct gindex_allocator alloc = setup_alloc();
	struct gindex gi1 = galloc(&alloc);
	gfree(&alloc, gi1);
	TEST_ASSERT(!islive(alloc, gi1));
}

void test_multiple_allocation_and_deallocation_conditions(void)
{
	struct gindex_allocator alloc = setup_alloc();
	struct gindex gis[10];
	for (int i = 0; i < 10; i++) {
		gis[i] = galloc(&alloc);
		TEST_ASSERT_EQUAL_INT(i, gis[i].index);
		TEST_ASSERT_EQUAL_INT(0, gis[i].gen);
	}

	gfree(&alloc, gis[4]);
	gfree(&alloc, gis[7]);

	struct gindex more_gis[3];
	for (int i = 0; i < 3; i++) {
		more_gis[i] = galloc(&alloc);
	}
	TEST_ASSERT_EQUAL_INT(7, more_gis[0].index);
	TEST_ASSERT_EQUAL_INT(1, more_gis[0].gen);
	TEST_ASSERT_EQUAL_INT(4, more_gis[1].index);
	TEST_ASSERT_EQUAL_INT(1, more_gis[1].gen);
	TEST_ASSERT_EQUAL_INT(10, more_gis[2].index);
	TEST_ASSERT_EQUAL_INT(0, more_gis[2].gen);
}

void test_islive_checks_for_generation_and_index(void)
{
	struct gindex_allocator alloc = setup_alloc();
	struct gindex gi = galloc(&alloc);
	gfree(&alloc, gi);
	struct gindex gi2 = galloc(&alloc);
	TEST_ASSERT(!islive(alloc, gi));
	TEST_ASSERT(islive(alloc, gi2));
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_galloc_should_return_generational_index);
	RUN_TEST(test_second_galloc_returns_incrementing_index);
	RUN_TEST(test_gfree_adds_free_index_to_pool);
	RUN_TEST(test_galloc_gallocs_free_index_if_available);
	RUN_TEST(test_islive_returns_true_if_index_in_use);
	RUN_TEST(test_islive_returns_false_if_index_gfreed);
	RUN_TEST(test_multiple_allocation_and_deallocation_conditions);
	RUN_TEST(test_islive_checks_for_generation_and_index);
	return UNITY_END();
}
