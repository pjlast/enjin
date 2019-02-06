#include "../unity/src/unity.h"
#include "../src/gamestate/gamestate.h"
#include "../src/gindex/gindex.h"
#include <stdio.h>

void test_init_gamestate_returns_initialized_gamestate(void)
{
	struct gamestate gs = init_gamestate();
	TEST_ASSERT_EQUAL_INT(0, gs.alloc.numents);
	TEST_ASSERT_EQUAL_INT(0, gs.alloc.numfree);
	TEST_ASSERT_EQUAL_INT(0, gs.numcomps);
	TEST_ASSERT_NULL(gs.ents);
	TEST_ASSERT_NULL(gs.comps);
}

void test_create_entity_adds_entity_to_gamestate(void)
{
	struct gamestate gs = init_gamestate();
	struct gindex entity = create_entity(&gs);
	TEST_ASSERT_EQUAL(0, entity.index);
	TEST_ASSERT_EQUAL(0, entity.gen);
	TEST_ASSERT_EQUAL(1, gs.alloc.numents);
	TEST_ASSERT_EQUAL(0, gs.ents[0].index);
}

void test_destroy_entity_removes_entity_from_gamestate(void)
{
	struct gamestate gs = init_gamestate();
	struct gindex entity = create_entity(&gs);
	destroy_entity(&gs, entity);
	TEST_ASSERT_EQUAL(1, gs.alloc.numfree);
	TEST_ASSERT(!islive(gs.alloc, entity));
}

void test_register_component_allocates_memory(void)
{
	struct gamestate gs = init_gamestate();
	TEST_ASSERT_NULL(gs.comps);
	const int COMP_INDEX = register_component(&gs);
	TEST_ASSERT_NOT_NULL(gs.comps);
	TEST_ASSERT_EQUAL_INT(1, gs.numcomps);
	TEST_ASSERT_NOT_NULL(gs.comps[COMP_INDEX]);
}

void test_create_and_free_multiple_ents(void)
{
	struct gamestate gs = init_gamestate();
	register_component(&gs);
	for (int i = 0; i < 20; i++) {
		struct gindex entity = create_entity(&gs);
		add_position(&gs, entity, 0, 1);
		TEST_ASSERT_EQUAL(gs.ents[i].index, entity.index);
		TEST_ASSERT_EQUAL(1, ((struct position**) gs.comps[0])[i]->y);
	}

	destroy_entity(&gs, gs.ents[5]);
	destroy_entity(&gs, gs.ents[9]);

	TEST_ASSERT(!islive(gs.alloc, gs.ents[5]));
	TEST_ASSERT(!islive(gs.alloc, gs.ents[9]));
	TEST_ASSERT_EQUAL(gs.comps[0][5], NULL);
	TEST_ASSERT_EQUAL(gs.comps[0][9], NULL);
}

int main(void)
{
	UNITY_BEGIN();
	RUN_TEST(test_init_gamestate_returns_initialized_gamestate);
	RUN_TEST(test_create_entity_adds_entity_to_gamestate);
	RUN_TEST(test_destroy_entity_removes_entity_from_gamestate);
	RUN_TEST(test_register_component_allocates_memory);
	RUN_TEST(test_create_and_free_multiple_ents);
	return UNITY_END();
}
