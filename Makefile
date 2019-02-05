CFLAGS  = -std=c99
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Werror

LFLAGS = -lSDL2
LFLAGS += -lSDL2_image

.PHONY: build test clean

test: test/gamestate.out test/gindex.out
	@./test/gamestate.out
	@./test/gindex.out

clean:
	rm -rf test/*.out
	rm -rf src/program

test/gamestate.out: test/test-gamestate.c
	@echo Compiling $@
	@$(CC) $(CFLAGS) unity/src/unity.c test/test-gamestate.c src/gamestate/gamestate.c src/gindex/gindex.c -o test/gamestate.out

test/gindex.out: test/test-gindex.c
	@echo Compiling $@
	@$(CC) $(CFLAGS) unity/src/unity.c test/test-gindex.c src/gindex/gindex.c -o test/gindex.out

build:
	@$(CC) $(CFLAGS) $(LFLAGS) -o src/program src/main.c src/gindex/gindex.c src/gamestate/gamestate.c src/components/physics/physics.c src/components/collision/collision.c
