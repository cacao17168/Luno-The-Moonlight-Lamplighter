#include <stdbool.h>
#include <SDL2/SDL.h>
#ifndef INPUT_H_
#define INPUT_H_

//keytype enum for tracking specific key states
typedef enum {
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_SPACE,
    KEY_F11,
    KEY_COUNT
} keytype;

//function to check if a key is pressed; returns 1 if pressed, 0 otherwise
int KeyPressed(SDL_Event* ev);

//function to update the keys array with current key states; returns 0 on success
int Getkey(keytype keys[]);

#endif
