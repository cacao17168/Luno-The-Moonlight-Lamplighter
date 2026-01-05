#include <SDL2/SDL.h>
#include "input.h" //header file

int KeyPressed(SDL_Event* ev) {
    if (ev->type == SDL_KEYDOWN) {
        return 1; //key is pressed
    }
    else {
        return 0; //key is not pressed
    }
}

int Getkey(keytype keys[]) {
    if (!keys) {
        return 1; //check for null pointer
    }
    
    const Uint8* SDLkeys = SDL_GetKeyboardState(NULL); //get current keyboard state

    keys[KEY_W] = SDLkeys[SDL_SCANCODE_W]; //
    keys[KEY_A] = SDLkeys[SDL_SCANCODE_A]; //
    keys[KEY_S] = SDLkeys[SDL_SCANCODE_S]; //update keys array based on SDL keyboard state
    keys[KEY_D] = SDLkeys[SDL_SCANCODE_D]; //
    keys[KEY_SPACE] = SDLkeys[SDL_SCANCODE_SPACE]; //
    keys[KEY_F11] = SDLkeys[SDL_SCANCODE_F11]; //
    return 0;
}
