#include "../test/unity.h"
#include "gindex.h"

struct gindex_allocator setup_gia(void)
{
	struct gindex_allocator gia;

	gia.num_free = 0;
	gia.num_entries = 0;
	gia.free = NULL;
	gia.entries = NULL;

	return gia;
}

void test_galloc_should_return_generational_index(void)
{
	struct gindex_allocator gia = setup_gia();
	struct gindex gi = galloc(&gia);
	TEST_ASSERT_EQUAL_INT(0, gi.index);
	TEST_ASSERT_EQUAL_INT(0, gi.gen);
}

void test_second_galloc_returns_incrementing_index(void)
{
	struct gindex_allocator gia = setup_gia();
	struct gindex gi1 = galloc(&gia);
	struct gindex gi2 = galloc(&gia);
	TEST_ASSERT_EQUAL_INT(0, gi1.index);
	TEST_ASSERT_EQUAL_INT(0, gi1.gen);
	TEST_ASSERT_EQUAL_INT(1, gi2.index);
	TEST_ASSERT_EQUAL_INT(0, gi2.gen);
}

void test_gfree_adds_free_index_to_pool(void)
{
	struct gindex_allocator gia = setup_gia();
	struct gindex gi1 = galloc(&gia);
	bool did_free = gfree(&gia, gi1);
	TEST_ASSERT(did_free);
	TEST_ASSERT_EQUAL_INT(1, gia.num_free);
	TEST_ASSERT_EQUAL_INT(0, gia.free[0]);
	TEST_ASSERT(!is_live(gia, gi1));
}

void test_galloc_gallocs_free_index_if_available(void)
{
	struct gindex_allocator gia = setup_gia();
	struct gindex gi1 = galloc(&gia);
	gfree(&gia, gi1);
	struct gindex gi2 = galloc(&gia);
	TEST_ASSERT_EQUAL_INT(0, gi2.index);
	TEST_ASSERT_EQUAL_INT(1, gi2.gen);
	TEST_ASSERT_EQUAL_INT(0, gia.num_free);
}

void test_is_live_returns_true_if_index_in_use(void)
{
	struct gindex_allocator gia = setup_gia();
	struct gindex gi1 = galloc(&gia);
	TEST_ASSERT(is_live(gia, gi1));
}

void test_is_live_returns_false_if_index_gfreed(void)
{
	struct gindex_allocator gia = setup_gia();
	struct gindex gi1 = galloc(&gia);
	gfree(&gia, gi1);
	TEST_ASSERT(!is_live(gia, gi1));
}

void test_multiple_allocation_and_deallocation_conditions(void)
{
	struct gindex_allocator gia = setup_gia();
	struct gindex gis[10];
	for (int i = 0; i < 10; i++) {
		gis[i] = galloc(&gia);
		TEST_ASSERT_EQUAL_INT(i, gis[i].index);
		TEST_ASSERT_EQUAL_INT(0, gis[i].gen);
	}

	gfree(&gia, gis[4]);
	gfree(&gia, gis[7]);

	struct gindex more_gis[3];
	for (int i = 0; i < 3; i++) {
		more_gis[i] = galloc(&gia);
	}
	TEST_ASSERT_EQUAL_INT(7, more_gis[0].index);
	TEST_ASSERT_EQUAL_INT(1, more_gis[0].gen);
	TEST_ASSERT_EQUAL_INT(4, more_gis[1].index);
	TEST_ASSERT_EQUAL_INT(1, more_gis[1].gen);
	TEST_ASSERT_EQUAL_INT(10, more_gis[2].index);
	TEST_ASSERT_EQUAL_INT(0, more_gis[2].gen);
}

void test_is_live_checks_for_generation_and_index(void)
{
	struct gindex_allocator gia = setup_gia();
	struct gindex gi = galloc(&gia);
	gfree(&gia, gi);
	struct gindex gi2 = galloc(&gia);
	TEST_ASSERT(!is_live(gia, gi));
	TEST_ASSERT(is_live(gia, gi2));
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_galloc_should_return_generational_index);
	RUN_TEST(test_second_galloc_returns_incrementing_index);
	RUN_TEST(test_gfree_adds_free_index_to_pool);
	RUN_TEST(test_galloc_gallocs_free_index_if_available);
	RUN_TEST(test_is_live_returns_true_if_index_in_use);
	RUN_TEST(test_is_live_returns_false_if_index_gfreed);
	RUN_TEST(test_multiple_allocation_and_deallocation_conditions);
	RUN_TEST(test_is_live_checks_for_generation_and_index);
	return UNITY_END();
}
