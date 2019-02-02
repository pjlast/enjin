test: tests.out
	@./tests.out

tests.out: test-gindex.c gindex.c
	$(CC) test-gindex.c gindex.c ../test/unity.c -o tests.out
