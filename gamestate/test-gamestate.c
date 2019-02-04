#include "../test/unity.h"
#include "gamestate.h"

void test_init_gamestate_returns_initialized_gamestate(void)
{
	struct gamestate gs = init_gamestate();
	TEST_ASSERT_EQUAL_INT(0, gs.entity_allocator.num_entries);
	TEST_ASSERT_EQUAL_INT(0, gs.entity_allocator.num_free);
	TEST_ASSERT_NOT_NULL(gs.entities);
}

void test_create_entity_adds_entity_to_gamestate(void)
{
	struct gamestate gs = init_gamestate();
	struct gindex entity = create_entity(&gs);
	TEST_ASSERT_EQUAL(0, entity.index);
	TEST_ASSERT_EQUAL(0, entity.gen);
	TEST_ASSERT_EQUAL(1, gs.entity_allocator.num_entries);
	TEST_ASSERT_EQUAL(0, gs.entities[0].index);
}

void test_create_and_free_multiple_entities(void)
{
	struct gamestate gs = init_gamestate();
	for (int i = 0; i < 20; i++) {
		struct gindex entity = create_entity(&gs);
		TEST_ASSERT_EQUAL(gs.entities[i].index, entity.index);
	}
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_init_gamestate_returns_initialized_gamestate);
	RUN_TEST(test_create_entity_adds_entity_to_gamestate);
	RUN_TEST(test_create_and_free_multiple_entities);
	return UNITY_END();
}
