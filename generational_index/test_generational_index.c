#include "../test/unity.h"
#include "generational_index.h"

generational_index_allocator_t setup_gia(void)
{
	generational_index_allocator_t gia;
	gia.num_free = 0;
	gia.num_entries = 0;

	return gia;
}

void test_allocate_should_return_generational_index(void)
{
	generational_index_allocator_t gia = setup_gia();
	generational_index_t gi = allocate(&gia);
	TEST_ASSERT_EQUAL_INT(0, gi.index);
	TEST_ASSERT_EQUAL_INT(0, gi.generation);
}

void test_second_allocate_returns_incrementing_index(void)
{
	generational_index_allocator_t gia = setup_gia();
	generational_index_t gi1 = allocate(&gia);
	generational_index_t gi2 = allocate(&gia);
	TEST_ASSERT_EQUAL_INT(0, gi1.index);
	TEST_ASSERT_EQUAL_INT(0, gi1.generation);
	TEST_ASSERT_EQUAL_INT(1, gi2.index);
	TEST_ASSERT_EQUAL_INT(0, gi2.generation);
}

void test_deallocate_adds_free_index_to_pool(void)
{
	generational_index_allocator_t gia = setup_gia();
	generational_index_t gi1 = allocate(&gia);
	bool did_free = deallocate(&gia, gi1);
	TEST_ASSERT(did_free);
	TEST_ASSERT_EQUAL_INT(1, gia.num_free);
	TEST_ASSERT_EQUAL_INT(0, gia.free[0]);
	TEST_ASSERT(!gia.entries[0].is_live);
}

void test_allocate_allocates_free_index_if_available(void)
{
	generational_index_allocator_t gia = setup_gia();
	generational_index_t gi1 = allocate(&gia);
	deallocate(&gia, gi1);
	generational_index_t gi2 = allocate(&gia);
	TEST_ASSERT_EQUAL_INT(0, gi2.index);
	TEST_ASSERT_EQUAL_INT(1, gi2.generation);
	TEST_ASSERT_EQUAL_INT(0, gia.num_free);
}

void test_is_live_returns_true_if_index_in_use(void)
{
	generational_index_allocator_t gia = setup_gia();
	generational_index_t gi1 = allocate(&gia);
	TEST_ASSERT(is_live(gia, gi1));
}

void test_is_live_returns_false_if_index_deallocated(void)
{
	generational_index_allocator_t gia = setup_gia();
	generational_index_t gi1 = allocate(&gia);
	deallocate(&gia, gi1);
	TEST_ASSERT(!is_live(gia, gi1));
}

void test_multiple_allocation_and_deallocation_conditions(void)
{
	generational_index_allocator_t gia = setup_gia();
	generational_index_t gis[10];
	for (int i = 0; i < 10; i++) {
		gis[i] = allocate(&gia);
		TEST_ASSERT_EQUAL_INT(i, gis[i].index);
		TEST_ASSERT_EQUAL_INT(0, gis[i].generation);
	}

	deallocate(&gia, gis[4]);
	deallocate(&gia, gis[7]);

	generational_index_t more_gis[3];
	for (int i = 0; i < 3; i++) {
		more_gis[i] = allocate(&gia);
	}
	TEST_ASSERT_EQUAL_INT(7, more_gis[0].index);
	TEST_ASSERT_EQUAL_INT(1, more_gis[0].generation);
	TEST_ASSERT_EQUAL_INT(4, more_gis[1].index);
	TEST_ASSERT_EQUAL_INT(1, more_gis[1].generation);
	TEST_ASSERT_EQUAL_INT(10, more_gis[2].index);
	TEST_ASSERT_EQUAL_INT(0, more_gis[2].generation);
}

void test_is_live_checks_for_generation_and_index(void)
{
	generational_index_allocator_t gia = setup_gia();
	generational_index_t gi = allocate(&gia);
	deallocate(&gia, gi);
	generational_index_t gi2 = allocate(&gia);
	TEST_ASSERT(!is_live(gia, gi));
	TEST_ASSERT(is_live(gia, gi2));
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_allocate_should_return_generational_index);
	RUN_TEST(test_second_allocate_returns_incrementing_index);
	RUN_TEST(test_deallocate_adds_free_index_to_pool);
	RUN_TEST(test_allocate_allocates_free_index_if_available);
	RUN_TEST(test_is_live_returns_true_if_index_in_use);
	RUN_TEST(test_is_live_returns_false_if_index_deallocated);
	RUN_TEST(test_multiple_allocation_and_deallocation_conditions);
	RUN_TEST(test_is_live_checks_for_generation_and_index);
	return UNITY_END();
}
