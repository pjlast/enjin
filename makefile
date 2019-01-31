CFLAGS = -lSDL2

build:
	$(CC) $(CFLAGS) -o program main.c
