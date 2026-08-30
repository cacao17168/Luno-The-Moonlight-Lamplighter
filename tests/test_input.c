#ifdef TEST

#include <stdio.h>
#include <string.h>
#include "input.h"

//mock SDL_GetKeyboardState
static Uint8 mock_keyboard_state[SDL_NUM_SCANCODES];

const Uint8* Input_GetKeyboardState(int* numkeys) {
    if(numkeys) *numkeys = SDL_NUM_SCANCODES;
    return mock_keyboard_state;
}

static void mock_reset(void) {
    memset(mock_keyboard_state, 0, sizeof(mock_keyboard_state));
}

//test framework
#define ASSERT_EQ(expected, actual) \
    do {\
        if((expected) != (actual)) {\
            printf("[FAIL] %s:%d: expected %d, actual %d\n", __FILE__, __LINE__, (int)(expected), (int)(actual));\
            return 1;\
        }\
    } while (0)

#define ASSERT_COND(cond) \
    do {\
        if(!(cond)) {\
            printf("[FAIL] %s: %d: asserting failed %s\n", __FILE__, __LINE__, #cond);\
            return 1;\
        }\
    } while(0)

typedef int (*test_func_t)(void);

static int run_test(test_func_t test, const char* name) {
    printf(" %-45s ", name);
    fflush(stdout);
    int rc = test();
    if(rc == 0) {
        printf("[PASS]\n");
        return 0;
    }
    return 1;
}

//tests for KeyPressed()
static int test_keypressed_keydown(void) {
    SDL_Event ev;
    ev.type = SDL_KEYDOWN;
    ASSERT_EQ(1, KeyPressed(&ev));
    return 0;
}

static int test_keypressed_keyup(void) {
    SDL_Event ev;
    ev.type = SDL_KEYUP;
    ASSERT_EQ(0, KeyPressed(&ev));
    return 0;
}

static int test_keypressed_mouse(void) {
    SDL_Event ev;
    ev.type = SDL_MOUSEBUTTONDOWN;
    ASSERT_EQ(0, KeyPressed(&ev));
    return 0;
}

//tests for Getkey();
static int test_getkey_null(void) {
    ASSERT_EQ(1, Getkey(NULL));
    return 0;
}

static int test_getkey_allreleased(void) {
    keytype keys[KEY_COUNT];
    mock_reset();
    ASSERT_EQ(0, Getkey(keys));
    for(int i = 0; i < KEY_COUNT; i++) {
        ASSERT_EQ(0, keys[i]);
    }
    return 0;
}

static int test_getkey_w_pressed(void) {
    keytype keys[KEY_COUNT];
    mock_reset();
    mock_keyboard_state[SDL_SCANCODE_W] = 1;
    ASSERT_EQ(0, Getkey(keys));
    ASSERT_EQ(1, keys[KEY_W]);
    ASSERT_EQ(0, keys[KEY_A]);
    ASSERT_EQ(0, keys[KEY_S]);
    ASSERT_EQ(0, keys[KEY_D]);
    ASSERT_EQ(0, keys[KEY_SPACE]);
    ASSERT_EQ(0, keys[KEY_F11]);
    return 0;
}

static int test_getkey_multiple_pressed(void) {
    keytype keys[KEY_COUNT];
    mock_reset();
    mock_keyboard_state[SDL_SCANCODE_A] = 1;
    mock_keyboard_state[SDL_SCANCODE_D] = 1;
    mock_keyboard_state[SDL_SCANCODE_SPACE] = 1;
    ASSERT_EQ(0, Getkey(keys));
    ASSERT_EQ(0, keys[KEY_W]);
    ASSERT_EQ(1, keys[KEY_A]);
    ASSERT_EQ(0, keys[KEY_S]);
    ASSERT_EQ(1, keys[KEY_D]);
    ASSERT_EQ(1, keys[KEY_SPACE]);
    ASSERT_EQ(0, keys[KEY_F11]);
    return 0;
}

static int test_getkey_f11_pressed(void) {
    keytype keys[KEY_COUNT];
    mock_reset();
    mock_keyboard_state[SDL_SCANCODE_F11] = 1;
    ASSERT_EQ(0, Getkey(keys));
    ASSERT_EQ(0, keys[KEY_W]);
    ASSERT_EQ(0, keys[KEY_A]);
    ASSERT_EQ(0, keys[KEY_S]);
    ASSERT_EQ(0, keys[KEY_D]);
    ASSERT_EQ(0, keys[KEY_SPACE]);
    ASSERT_EQ(1, keys[KEY_F11]);
    return 0;
}

int main(void) {
    printf("Running input module tests...\n");
    int failures = 0;
    failures += run_test(test_keypressed_keydown, "KeyPressed(KEYDOWN)");
    failures += run_test(test_keypressed_keyup, "KeyPressed(KEYUP)");
    failures += run_test(test_keypressed_mouse, "KeyPressed(MOUSEBUTTONDOWN)");
    failures += run_test(test_getkey_null, "Getkey(null)");
    failures += run_test(test_getkey_allreleased, "Getkey(all released)");
    failures += run_test(test_getkey_w_pressed, "Getkey(w pressed)");
    failures += run_test(test_getkey_multiple_pressed, "Getkey(A+D+Space pressed)");
    failures += run_test(test_getkey_f11_pressed, "GetKet(f11 pressed)");

    printf("Tests ended. Passed: %d/%d\n", 8 - failures, 8);
    return 0;
}
#endif