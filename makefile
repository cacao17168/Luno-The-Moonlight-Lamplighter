MAIN = ./src/engine/game/game.c
INPUT = ./src/engine/input/input.c
RENDERER = ./src/engine/renderer/renderer.c
PHYSICS = ./src/engine/physics/physics.c

INCLUDES = -I./src/engine/game -I./src/engine/input -I./src/engine/renderer -I./src/engine/physics

CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_image)
LIBS = $(shell pkg-config --libs sdl2 SDL2_image)

FLAGS = -g -O0 -lm

NAME = 'Luno: The Moonlight Lamplighter'

compile: $(MAIN) $(SUB)
	gcc -o $(NAME) $(MAIN) $(INPUT) $(RENDERER) $(PHYSICS) $(INCLUDES) $(CFLAGS) $(FLAGS) $(LIBS)
