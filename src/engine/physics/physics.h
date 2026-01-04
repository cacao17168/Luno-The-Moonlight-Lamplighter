#ifndef PHYSICS_H_
#define PHYSICS_H_
#include "input.h" //for keytype enum
#include <SDL2/SDL.h>

extern bool fullscreen; //variable to track fullscreen state

/* game_properties is a structure containing map size, path to background sprite, blocks data for collisions(a dynamic array), count of blocks, and camera position */

typedef struct {
    SDL_Rect Size;
    char *BackgroundPath;
    SDL_Rect *Blocks;
    int BlocksCount;
    SDL_Rect Camera;
} game_properties;

//entity jump state enum

typedef enum {
    falling,
    lifting
} entity_jump_state;

//player structure containing vertical velocity, jump state, hitbox, health, speed, and last movement direction

typedef struct {
    float vy;
    entity_jump_state state;
    SDL_Rect hitbox;
    int health;
    float speed;
    int last_direction;
} player;

//as arguments, takes width and height of the map, path to background sprite, and camera rectangle(only with width and height of camera); returns initialized game_properties structure

game_properties game_properties_Init(int width, int height, const char *path, SDL_Rect Camera);

int update(keytype keys[], float dt, player* Pl, SDL_Window* window, game_properties *Props);

#endif
