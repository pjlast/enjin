test: tests.out
	@./tests.out

tests.out: test-gamestate.c gamestate.c
	$(CC) -std=c99 ../gindex/gindex.c test-gamestate.c gamestate.c ../test/unity.c -o tests.out
