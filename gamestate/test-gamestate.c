#include "../test/unity.h"
#include "gamestate.h"

void test_init_gamestate_returns_initialized_gamestate(void)
{
	struct gamestate gs = init_gamestate();
	TEST_ASSERT_EQUAL_INT(0, gs.entity_allocator.num_entries);
	TEST_ASSERT_EQUAL_INT(0, gs.entity_allocator.num_free);
	TEST_ASSERT_NULL(gs.entities);
	TEST_ASSERT_NOT_NULL(gs.positions);
	TEST_ASSERT_NOT_NULL(gs.draws);
}

void test_create_entity_adds_entity_to_gamestate(void)
{
	struct gamestate gs = init_gamestate();
	struct gindex entity = create_entity(&gs);
	TEST_ASSERT_EQUAL(0, entity.index);
	TEST_ASSERT_EQUAL(0, entity.gen);
	TEST_ASSERT_EQUAL(1, gs.entity_allocator.num_entries);
	TEST_ASSERT_NULL(gs.positions[entity.index]);
	TEST_ASSERT_NULL(gs.draws[entity.index]);
}

void test_create_two_entities_indexes_and_components(void)
{
	struct gamestate gs = init_gamestate();
	for (int i = 0; i < 2; i++) {
		struct gindex entity = create_entity(&gs);
		TEST_ASSERT_EQUAL(i, entity.index);
		TEST_ASSERT_EQUAL(0, entity.gen);
		TEST_ASSERT_NULL(gs.positions[entity.index]);
		TEST_ASSERT_NULL(gs.draws[entity.index]);
	}
	TEST_ASSERT_EQUAL(2, gs.entity_allocator.num_entries);
}

void test_add_position_creates_accessible_position(void)
{
	struct gamestate gs = init_gamestate();
	struct gindex entity = create_entity(&gs);
	add_position(&gs, entity, 1, 5);
	TEST_ASSERT_EQUAL(1, gs.positions[entity.index]->x);
	TEST_ASSERT_EQUAL(5, gs.positions[entity.index]->y);
}

void test_add_two_positions_creates_both_accessible(void)
{
	struct gamestate gs = init_gamestate();
	for (int i = 0; i < 2; i++) {
		struct gindex entity = create_entity(&gs);
		add_position(&gs, entity, i, i*2);
		TEST_ASSERT_EQUAL(i, gs.positions[entity.index]->x);
		TEST_ASSERT_EQUAL(i*2, gs.positions[entity.index]->y);
	}
}

void test_add_position_to_second_entity_leaves_first_one_null(void)
{
	struct gamestate gs = init_gamestate();
	struct gindex entities[2];
	for (int i = 0; i < 2; i++) {
		entities[i] = create_entity(&gs);
	}
	add_position(&gs, entities[1], 1, 5);
	TEST_ASSERT_NULL(gs.positions[0]);
	TEST_ASSERT_EQUAL(1, gs.positions[1]->x);
	TEST_ASSERT_EQUAL(5, gs.positions[1]->y);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_init_gamestate_returns_initialized_gamestate);
	RUN_TEST(test_create_entity_adds_entity_to_gamestate);
	RUN_TEST(test_create_two_entities_indexes_and_components);
	RUN_TEST(test_add_two_positions_creates_both_accessible);
	RUN_TEST(test_add_position_to_second_entity_leaves_first_one_null);
	return UNITY_END();
}
