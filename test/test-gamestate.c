#include "../unity/src/unity.h"
#include "../src/gamestate/gamestate.h"
#include "../src/gindex/gindex.h"
#include <stdio.h>

void test_init_gamestate_returns_initialized_gamestate(void)
{
	struct gamestate gs = init_gamestate();
	TEST_ASSERT_EQUAL_INT(0, gs.allocator.num_entries);
	TEST_ASSERT_EQUAL_INT(0, gs.allocator.num_free);
	TEST_ASSERT_EQUAL_INT(0, gs.num_components);
	TEST_ASSERT_NULL(gs.entities);
	TEST_ASSERT_NULL(gs.components);
}

void test_create_entity_adds_entity_to_gamestate(void)
{
	struct gamestate gs = init_gamestate();
	struct gindex entity = create_entity(&gs);
	TEST_ASSERT_EQUAL(0, entity.index);
	TEST_ASSERT_EQUAL(0, entity.gen);
	TEST_ASSERT_EQUAL(1, gs.allocator.num_entries);
	TEST_ASSERT_EQUAL(0, gs.entities[0].index);
}

void test_destroy_entity_removes_entity_from_gamestate(void)
{
	struct gamestate gs = init_gamestate();
	struct gindex entity = create_entity(&gs);
	destroy_entity(&gs, entity);
	TEST_ASSERT_EQUAL(1, gs.allocator.num_free);
	TEST_ASSERT(!is_live(gs.allocator, entity));
}

void test_register_component_allocates_memory(void)
{
	struct gamestate gs = init_gamestate();
	TEST_ASSERT_NULL(gs.components);
	const int COMP_INDEX = register_component(&gs);
	TEST_ASSERT_NOT_NULL(gs.components);
	TEST_ASSERT_EQUAL_INT(1, gs.num_components);
	TEST_ASSERT_NOT_NULL(gs.components[COMP_INDEX]);
}

void test_create_and_free_multiple_entities(void)
{
	struct gamestate gs = init_gamestate();
	register_component(&gs);
	for (int i = 0; i < 20; i++) {
		struct gindex entity = create_entity(&gs);
		add_position(&gs, entity, 0, 1);
		TEST_ASSERT_EQUAL(gs.entities[i].index, entity.index);
		TEST_ASSERT_EQUAL(1, ((struct position**) gs.components[0])[i]->y);
	}
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_init_gamestate_returns_initialized_gamestate);
	RUN_TEST(test_create_entity_adds_entity_to_gamestate);
	RUN_TEST(test_destroy_entity_removes_entity_from_gamestate);
	RUN_TEST(test_register_component_allocates_memory);
	RUN_TEST(test_create_and_free_multiple_entities);
	return UNITY_END();
}
