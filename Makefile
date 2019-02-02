CFLAGS = -std=c99 -lSDL2 -lSDL2_image

test: gindex/gindex.c gindex/test-gindex.c
	cd gindex && make test
build:
	$(CC) $(CFLAGS) -o program main.c
