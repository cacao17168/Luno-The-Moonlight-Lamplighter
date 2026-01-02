#include "physics.h"
#include "input.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define JUMP_SCALE 100
#define ACCELERATION 500

bool fullscreen = false;

game_properties game_properties_Init(int width, int height, const char *path, SDL_Rect Camera) {
game_properties InitVar = {0};

    InitVar.Size = (SDL_Rect){0};
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

void jump(player* plyr, float dt, game_properties *Props, int center) {
    int camera_center_h = Props->Camera.h / 2 + Props->Camera.y;
    
    plyr->hitbox.y -= ceil(plyr->vy * dt);

    if(center)
    Props->Camera.y = plyr->hitbox.y - Props->Camera.h / 2;

    plyr->vy -= 7.5f;
    printf("jump changed: %f\n", ceil(plyr->vy * dt));
    printf("y = %d\n", plyr->hitbox.y);
    printf("jump processed\n");
}

int update(keytype keys[], float dt, player* Pl, SDL_Window* window, game_properties *Props) {
    int grnd = Props->Size.h - Pl->hitbox.h;

    int camera_center_h = Props->Camera.h / 2 + Props->Camera.y;
    int camera_center_w = Props->Camera.w / 2 + Props->Camera.x;

    int center_h = 0;
    int center_w = 0;

    int left_border = 0;
    int right_border = 0;
    int top_border = 0;
    int bottom_border = 0;
    
    Pl->last_direction = (keys[KEY_A] ? -1 : (keys[KEY_D] ? 1 : 0));

    if(abs(Pl->hitbox.x - camera_center_w) <= 4) center_w = 1;
    if(abs(Pl->hitbox.y - camera_center_h) <= 4) center_h = 1;

    if(Props->Camera.x <= 0) left_border = 1;
    if(Props->Camera.x + Props->Camera.w >= Props->Size.w) right_border = 1;
    if(Props->Camera.y <= 0) top_border = 1;
    if(Props->Camera.y + Props->Camera.h >= Props->Size.h) bottom_border = 1;

    if (keys[KEY_A] && !keys[KEY_D]) {
        if(Pl->last_direction != -1) Pl->speed = 0;

        Pl->speed += (ACCELERATION * dt) ;
        
        if (Pl->speed > 150) Pl->speed = 150;
        printf("x a before: %d\n", Pl->hitbox.x);
        
        Pl->hitbox.x -= floor(Pl->speed * dt) ;
        
        if(center_w && !left_border)
        Props->Camera.x = Pl->hitbox.x - Props->Camera.w / 2;

        printf("x a after: %d\n", Pl->hitbox.x);
        printf("a: %f\n", Pl->speed * dt);
    }

    if (keys[KEY_D] && !keys[KEY_A]) {
        if(Pl->last_direction != 1) Pl->speed = 0;
        
        Pl->speed += ACCELERATION * dt;
        
        if (Pl->speed > 150) Pl->speed = 150;
        printf("x d before: %d\n", Pl->hitbox.x);
        
        Pl->hitbox.x += floor(Pl->speed * dt);

        if(center_w && !right_border)
        Props->Camera.x = Pl->hitbox.x - Props->Camera.w / 2;

        printf("x d after: %d\n", Pl->hitbox.x);
        printf("d: %f\n", Pl->speed * dt);
    }
    
    if (!keys[KEY_A] && !keys[KEY_D]) {
        Pl->speed = 0;
    } else if (keys[KEY_A] && keys[KEY_D]) {
        Pl->speed = 0;
    }
    if (Pl->hitbox.y >= grnd) Pl->vy = 0;
    
    if (keys[KEY_SPACE]) {
        printf("space catched\n");
        if (Pl->hitbox.y >= grnd) {
            Pl->hitbox.y = grnd;
            Pl->state = lifting;
            Pl->vy = 250.0f;
            printf("ground check completed\n");
        }
    }
    
    if (Pl->state == lifting) jump(Pl, dt, Props, center_h);
    if (Pl->hitbox.y <= grnd - JUMP_SCALE) Pl->state = falling;
    printf("state processed\n");
    
    if (Pl->state == falling && Pl->hitbox.y <= grnd) {
        Pl->vy += 1.0f;
        if (Pl->vy > 75.0f) Pl->vy = 75.0f;
        
        Pl->hitbox.y += ceil(Pl->vy * dt);
    
        if(center_h && !bottom_border)
        Props->Camera.y = Pl->hitbox.y - Props->Camera.h / 2;

        printf("falled %f pixels\n", ceil(Pl->vy * dt));
        printf("y = %d\n", Pl->hitbox.y);
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
