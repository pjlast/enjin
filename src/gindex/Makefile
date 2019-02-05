test: tests.out
	@./tests.out

tests.out: test-gindex.c gindex.c
	$(CC) -std=c99 test-gindex.c gindex.c ../test/unity.c -o tests.out
