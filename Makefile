CFLAGS = -std=c99 -lSDL2 -lSDL2_image

tests:
	(cd gindex; make test)
build:
	$(CC) $(CFLAGS) -o program components/collision/collision.c components/physics/physics.c gindex/gindex.c gamestate/gamestate.c main.c
