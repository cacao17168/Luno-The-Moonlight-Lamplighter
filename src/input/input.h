#ifndef INPUT_H_
#define INPUT_H_

typedef enum {
    KEY_W,
    KEY_A,
    KEY_S,
    KEY_D,
    KEY_SPACE,
    KEY_F11,
    KEY_ESC,
    KEY_MOUSE_LMB,
    KEY_COUNT
} kb_key_t;

int get_keys(kb_key_t *keys);

#endif
