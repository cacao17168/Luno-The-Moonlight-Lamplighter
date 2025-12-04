#include <stdbool.h>
#ifndef INPUT_H_
#define INPUT_H_

static bool fullscreen;

typedef enum {
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_SPACE,
    KEY_F11,
    KEY_NONE
} keytype;

int InputInit();

bool KeyPressed();

keytype Getkey();

#endif
