#include "physics.h" //header file
#include "input.h" //for keytype enum
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

int fullscreen = 1; //variable to track fullscreen state

game_properties game_properties_Init(int width, int height, const char *path, SDL_Rect Camera) {
    game_properties InitVar = {0};

    InitVar.Size = (SDL_Rect){0}; //initialize Size to zero
    InitVar.Size.w = width; //set width
    InitVar.Size.h = height; //set height
    InitVar.BackgroundPath = path; //set background path
    InitVar.BlocksCount = 1; //initialize block count to 1
    InitVar.Blocks = malloc(sizeof(SDL_Rect)); //allocate memory for 1 block; 
    InitVar.Camera = Camera; //set camera
    
    return InitVar; //return initialized structure
}

int handle_fullscreen_toggle(SDL_Window* window, keytype keys[], int* fullscreen_state) {
    if(keys[KEY_F11]) {
        if(!fullscreen_state) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); //set fullscreen
            fullscreen_state = 0;
        } else {
            SDL_SetWindowFullscreen(window, 0); //exit fullscreen
            fullscreen_state = 1;
        }
    }
    return 0;
}

int update(keytype keys[], float dt, player* Pl, SDL_Window* window, game_properties *Props) {
    if (keys[KEY_SPACE]) {
        if (Pl->hitbox.y >= grnd) {
        }
    }
    
    if (Pl->state == lifting) jump(Pl, dt, Props, center_h); //handle jumping logic
    if (Pl->hitbox.y <= grnd - JUMP_SCALE) Pl->state = falling; //switch to falling state if maximum jump height is reached
    
    if (Pl->state == falling && Pl->hitbox.y <= grnd) {
        Pl->vy += 1.0f; //increase vertical velocity to simulate gravity
        if (Pl->vy > 75.0f) Pl->vy = 75.0f; //cap vertical velocity to maximum fall speed
        
        Pl->hitbox.y += ceil(Pl->vy * dt); //update player y position based on vertical velocity and delta time
    }
}
