MAIN = ./src/engine/game/game.c
INPUT = ./src/engine/input/input.c
RENDERER = ./src/engine/renderer/renderer.c
PHYSICS = ./src/engine/physics/physics.c

TEST_SRC = ./tests/test_input.c ./src/engine/input/input.c
TEST_BIN = test_input
TEST_INC = -I./src/engine/input -I./tests
TEST_FLAGS = -DTEST

INCLUDES = -I./src/engine/game -I./src/engine/input -I./src/engine/renderer -I./src/engine/physics

SDLFLAGS = $(shell pkg-config --cflags sdl2 SDL2_image)
LIBS = $(shell pkg-config --libs sdl2 SDL2_image)

CFLAGS = -Wall -Wextra -g -O0 -lm

NAME = -o Luno_The_Moonlight_Lamplighter

.PHONY: test clean

compile: $(MAIN)
	gcc $(NAME) $(MAIN) $(INPUT) $(RENDERER) $(PHYSICS) $(INCLUDES) $(SDLFLAGS) $(CFLAGS) $(LIBS)

$(TEST_BIN): $(TEST_SRC) ./src/engine/input/input.h
	gcc $(CFLAGS) $(SDLFLAGS) $(LIBS) $(TEST_SRC) $(TEST_INC) $(TEST_FLAGS) -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

clean: 
	rm -f $(TEST_BIN)
