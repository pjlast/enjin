test: tests.out
	@./tests.out

tests.out: test_generational_index.c generational_index.c
	$(CC) test_generational_index.c generational_index.c ../test/unity.c -o tests.out
