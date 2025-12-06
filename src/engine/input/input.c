#include <SDL2/SDL.h>
#include "input.h"

int KeyPressed(SDL_Event* ev) {
    if (ev->type == SDL_KEYDOWN) {
        return 1;
    }
    else {
        return 0; 
    }
}

int Getkey(keytype keys[]) {
    if (!keys) {
        return 1;
    }
    
    const Uint8* SDLkeys = SDL_GetKeyboardState(NULL);

    keys[KEY_W] = SDLkeys[SDL_SCANCODE_W];
    keys[KEY_A] = SDLkeys[SDL_SCANCODE_A];
    keys[KEY_S] = SDLkeys[SDL_SCANCODE_S];
    keys[KEY_D] = SDLkeys[SDL_SCANCODE_D];
    keys[KEY_SPACE] = SDLkeys[SDL_SCANCODE_SPACE];
    keys[KEY_F11] = SDLkeys[SDL_SCANCODE_F11];
    return 0;
}
