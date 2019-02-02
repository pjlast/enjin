CFLAGS = -std=c99 -lSDL2 -lSDL2_image

tests:
	(cd gindex; make test)
build:
	$(CC) $(CFLAGS) -o program gindex/gindex.c main.c
