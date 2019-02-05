CFLAGS  = -std=c99
CFLAGS += -g
CFLAGS += -Wall
CFLAGS += -Wextra
CFLAGS += -pedantic
CFLAGS += -Werror

LFLAGS = -lSDL2
LFLAGS += -lSDL2_image

test: tests/gamestate.out tests/gindex.out
	@./tests/gamestate.out
	@./tests/gindex.out

clean:
	rm -rf tests/*.out
	rm -rf src/program

tests/gamestate.out: tests/test-gamestate.c
	@echo Compiling $@
	@$(CC) $(CFLAGS) unity/src/unity.c tests/test-gamestate.c src/gamestate/gamestate.c src/gindex/gindex.c -o tests/gamestate.out

tests/gindex.out: tests/test-gindex.c
	@echo Compiling $@
	@$(CC) $(CFLAGS) unity/src/unity.c tests/test-gindex.c src/gindex/gindex.c -o tests/gindex.out

executable:
	@$(CC) $(CFLAGS) $(LFLAGS) -o src/program src/main.c src/gindex/gindex.c src/gamestate/gamestate.c src/components/physics/physics.c src/components/collision/collision.c
