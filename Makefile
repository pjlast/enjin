CFLAGS = -std=c99 -lSDL2 -lSDL2_image

tests:
	(cd gindex; make test)
build:
	$(CC) $(CFLAGS) -o program main.c
