#include "physics.h"
#include "input.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define JUMP_SCALE 100
#define ACCELERATION 500

float speed = 0;

void jump(int jmpspd, player* plyr, float dt) {
    plyr->y -= ceil(jmpspd * dt);
    printf("jump changed: %f\n", ceil(jmpspd * dt));
    printf("jump processed\n");
}

int update(keytype key, float dt, player* Pl, SDL_Window* window) {
const Uint8* keys = SDL_GetKeyboardState(NULL); 
    if (keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) {
        speed += (ACCELERATION * dt) ;
        
        if (speed > 150) speed = 150;
        printf("x a before: %d\n", Pl->x);
        
        Pl->x -= floor(speed * dt) ;
        
        printf("x a after: %d\n", Pl->x);
        printf("a: %f\n", speed * dt);
    }

    if (keys[SDL_SCANCODE_D] && !keys[SDL_SCANCODE_A]) {
        speed += ACCELERATION * dt;
        
        if (speed > 150) speed = 150;
        printf("x d before: %d\n", Pl->x);
        
        Pl->x += floor(speed * dt);
        
        printf("x d after: %d\n", Pl->x);
        printf("d: %f\n", speed * dt);
    }
    
    if (!keys[SDL_SCANCODE_A] && !keys[SDL_SCANCODE_D]) speed = 0;
    if (!keys[SDL_SCANCODE_SPACE] && Pl->y == 300 - 32) Pl->vy = 0;
    
    if (keys[SDL_SCANCODE_SPACE]) {
        printf("space catched\n");
        if (Pl->y == 300 - 32) {
            Pl->state = lifting;
            printf("ground check completed\n");
        }
    }
    
    if (Pl->state == lifting) jump(75, Pl, dt);
    if (Pl->y <= 300 - 32 - JUMP_SCALE) Pl->state = falling;
    printf("state processed\n");
    
    if (Pl->state == falling && Pl->y != 300 - 32) {
        Pl->vy += 25.0f;
        if (Pl->vy > 75.0f) Pl->vy = 75.0f;
        
        Pl->y += ceil(Pl->vy * dt);
        printf("falled %f pixels\n", ceil(Pl->vy * dt));
        printf("jump processed\n");
    }
    
    if (key == KEY_F11) {
        //printf("Key catched\n");
        if (!fullscreen) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
            //printf("fullscreen set\n");
            fullscreen = true; 
        } else {
            SDL_SetWindowFullscreen(window, 0);
            //printf("fullscreen disabled\n");
            fullscreen = false;
        }
    }
}
