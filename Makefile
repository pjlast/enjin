CFLAGS = -lSDL2 -lSDL2_image

test: gindex/tests.out
	cd gindex && make test
build:
	$(CC) $(CFLAGS) -o program main.c
