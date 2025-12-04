#include <stdbool.h>
#include <SDL2/SDL.h>

#include "input.h"

//const Uint8* keys = SDL_GetKeyboardState(NULL);

bool KeyPressed(SDL_Event* ev) {
    if (ev->type == SDL_KEYDOWN) {
        return true;
    }
    else {
        return false; 
    }
}

keytype Getkey(SDL_Event* event) {
    switch(event->key.keysym.scancode) {
        case SDL_SCANCODE_W:
            return KEY_W;
            
        case SDL_SCANCODE_A:
            return KEY_A;
        
        case SDL_SCANCODE_S:
            return KEY_S;
        
        case SDL_SCANCODE_D:
            return KEY_D;
        
        case SDL_SCANCODE_SPACE:
            return KEY_SPACE;
            
        case SDL_SCANCODE_F11:
            fullscreen = true;
            return KEY_F11;
        default:
            return KEY_NONE;
    }
}
