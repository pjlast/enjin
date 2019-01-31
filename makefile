CFLAGS = -lSDL2 -lSDL2_image

build:
	$(CC) $(CFLAGS) -o program main.c
