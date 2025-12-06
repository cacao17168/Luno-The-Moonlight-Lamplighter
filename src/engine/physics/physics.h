#ifndef PHYSICS_H_
#define PHYSICS_H_
#include "input.h"
#include <SDL2/SDL.h>

static bool fullscreen;

typedef enum {
    falling,
    lifting
} entity_jump_state;

typedef struct {
    int x;
    int y;
    float vy;
    entity_jump_state state;
} player;

//only 1 function will need in main file
int update(keytype keys[], float dt, player* Pl, SDL_Window* window);

#endif
