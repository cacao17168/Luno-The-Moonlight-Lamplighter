#include "physics.h"
#include "input.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define JUMP_SCALE 100
#define ACCELERATION 500

float speed = 0;
bool fullscreen = false;

game_properties game_properties_Init(int width, int height, const char *path, SDL_Rect Camera) {
game_properties InitVar = {0};

    InitVar.Size.w = width;
    InitVar.Size.h = height;
    InitVar.BackgroundPath = path;
    InitVar.BlocksCount = 1;
    InitVar.Blocks = malloc(sizeof(SDL_Rect));
    InitVar.Camera = Camera;
    
    return InitVar;
}

int isCollision() {
    
}

void jump(player* plyr, float dt, game_properties *Props) {
    plyr->hitbox.y -= ceil(plyr->vy * dt);
    Props->Camera.y -= ceil(plyr->vy * dt);
    plyr->vy -= 7.5f;
    printf("jump changed: %f\n", ceil(plyr->vy * dt));
    printf("jump processed\n");
}

int update(keytype keys[], float dt, player* Pl, SDL_Window* window, game_properties *Props) { 

    if (keys[KEY_A] && !keys[KEY_D]) {
        speed += (ACCELERATION * dt) ;
        
        if (speed > 150) speed = 150;
        printf("x a before: %d\n", Pl->hitbox.x);
        
        Pl->hitbox.x -= floor(speed * dt) ;
        
        printf("x a after: %d\n", Pl->hitbox.x);
        printf("a: %f\n", speed * dt);
    }

    if (keys[KEY_D] && !keys[KEY_A]) {
        speed += ACCELERATION * dt;
        
        if (speed > 150) speed = 150;
        printf("x d before: %d\n", Pl->hitbox.x);
        
        Pl->hitbox.x += floor(speed * dt);
        Props->Camera.x += floor(speed * dt);
        
        printf("x d after: %d\n", Pl->hitbox.x);
        printf("d: %f\n", speed * dt);
    }
    
    if (!keys[KEY_A] && !keys[KEY_D]) speed = 0;
    if (Pl->hitbox.y == Props->Size.y - Pl->hitbox.y) Pl->vy = 0;
    
    if (keys[KEY_SPACE]) {
        printf("space catched\n");
        if (Pl->hitbox.y == Props->Size.y - Pl->hitbox.y) {
            Pl->state = lifting;
            Pl->vy = 250.0f;
            printf("ground check completed\n");
        }
    }
    
    if (Pl->state == lifting) jump(Pl, dt, Props);
    if (Pl->hitbox.y <= Props->Size.y - Pl->hitbox.y - JUMP_SCALE) Pl->state = falling;
    printf("state processed\n");
    
    if (Pl->state == falling && Pl->hitbox.y != Props->Size.y - Pl->hitbox.y) {
        Pl->vy += 1.0f;
        if (Pl->vy > 75.0f) Pl->vy = 75.0f;
        
        Pl->hitbox.y += ceil(Pl->vy * dt);
        printf("falled %f pixels\n", ceil(Pl->vy * dt));
        printf("jump processed\n");
    }
    
    if (keys[KEY_F11]) {
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
    return 0;
}
