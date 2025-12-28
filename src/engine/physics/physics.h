#ifndef PHYSICS_H_
#define PHYSICS_H_
#include "input.h"
#include <SDL2/SDL.h>

extern bool fullscreen;

typedef struct {
    SDL_Rect Size;
    char *BackgroundPath;
    SDL_Rect *Blocks;
    int BlocksCount;
    SDL_Rect Camera;
} game_properties;

typedef enum {
    falling,
    lifting
} entity_jump_state;

typedef struct {
    float vy;
    entity_jump_state state;
    SDL_Rect hitbox;
    int health;
} player;

game_properties game_properties_Init(int width, int height, const char *path, SDL_Rect Camera);

//only 1 function will need in main file
int update(keytype keys[], float dt, player* Pl, SDL_Window* window, game_properties *Props);

#endif
