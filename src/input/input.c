#include "input.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_scancode.h>

int get_keys(kb_key_t *keys) {
    if(!keys) return 1;

    SDL_PumpEvents();
    const Uint8 *SDLkeys = SDL_GetKeyboardState(NULL);
    Uint32 SDLmkeys = SDL_GetMouseState(NULL, NULL);

    keys[KEY_W] = SDLkeys[SDL_SCANCODE_W];
    keys[KEY_A] = SDLkeys[SDL_SCANCODE_A];
    keys[KEY_S] = SDLkeys[SDL_SCANCODE_S];
    keys[KEY_D] = SDLkeys[SDL_SCANCODE_D];
    keys[KEY_SPACE] = SDLkeys[SDL_SCANCODE_SPACE];
    keys[KEY_F11] = SDLkeys[SDL_SCANCODE_F11];
    keys[KEY_ESC] = SDLkeys[SDL_SCANCODE_ESCAPE];
    keys[KEY_MOUSE_LMB] = (SDLmkeys & SDL_BUTTON_LMASK) != 0;
    return 0;
}
