#include "physics.h" //header file
#include "input.h" //for keytype enum
#include <SDL2/SDL.h>
#include <stdio.h>
#include <math.h>

#define JUMP_SCALE 100 //maximum jump height
#define ACCELERATION 500 //player acceleration

bool fullscreen = false; //variable to track fullscreen state

game_properties game_properties_Init(int width, int height, const char *path, SDL_Rect Camera) {
game_properties InitVar = {0};

    InitVar.Size = (SDL_Rect){0}; //initialize Size to zero
    InitVar.Size.w = width; //set width
    InitVar.Size.h = height; //set height
    InitVar.BackgroundPath = path; //set background path
    InitVar.BlocksCount = 1; //initialize block count to 1
    InitVar.Blocks = malloc(sizeof(SDL_Rect)); //allocate memory for 1 block; 
    InitVar.Camera = Camera; //set camera
    
    return InitVar; //return initialized structure
}

int isCollision() {
    //a prototipe of function for future collision detection implementation
}

//function to handle jumping logic; as arguments, takes a pointer to player structure, delta time, pointer to game_properties structure, and a center flag

void jump(player* plyr, float dt, game_properties *Props, int center) {
    int camera_center_h = Props->Camera.h / 2 + Props->Camera.y; //get camera center height
    
    plyr->hitbox.y -= ceil(plyr->vy * dt); //update player y position based on vertical velocity and delta time

    if(center)
    Props->Camera.y = plyr->hitbox.y - Props->Camera.h / 2; //update camera y position if centered

    plyr->vy -= 7.5f; //decrease vertical velocity to simulate gravity
    printf("jump changed: %f\n", ceil(plyr->vy * dt)); //
    printf("y = %d\n", plyr->hitbox.y);                // debug info
    printf("jump processed\n");                        //
}

int update(keytype keys[], float dt, player* Pl, SDL_Window* window, game_properties *Props) {
    int grnd = Props->Size.h - Pl->hitbox.h; //calculate ground level based on map height and player height

    int camera_center_h = Props->Camera.h / 2 + Props->Camera.y; //get camera center height
    int camera_center_w = Props->Camera.w / 2 + Props->Camera.x; //get camera center width

    int center_h = 0; //flags to check if player is centered in camera for y movements; may be only 1 or 0
    int center_w = 0; //flags to check if player is centered in camera for x movements; may be only 1 or 0

    int left_border = 0; //flags to check if camera is at map left border; may be only 1 or 0
    int right_border = 0; //flags to check if camera is at map right border; may be only 1 or 0
    int top_border = 0; //flags to check if camera is at map top border; may be only 1 or 0
    int bottom_border = 0; //flags to check if camera is at map bottom border; may be only 1 or 0
    
    Pl->last_direction = (keys[KEY_A] ? -1 : (keys[KEY_D] ? 1 : 0)); //update last direction based on pressed keys; -1 for left, 1 for right, 0 for none

    if(abs(Pl->hitbox.x - camera_center_w) <= 4) center_w = 1; //check if player is centered in camera for x movements
    if(abs(Pl->hitbox.y - camera_center_h) <= 4) center_h = 1; //check if player is centered in camera for y movements

    if(Props->Camera.x <= 0) left_border = 1; //check if camera is at map left border
    if(Props->Camera.x + Props->Camera.w >= Props->Size.w) right_border = 1; //check if camera is at map right border

    if(Props->Camera.y <= 0) top_border = 1; //check if camera is at map top border
    if(Props->Camera.y + Props->Camera.h >= Props->Size.h) bottom_border = 1; //check if camera is at map bottom border

    if (keys[KEY_A] && !keys[KEY_D]) {
        if(Pl->last_direction != -1) Pl->speed = 0; //reset speed if changing direction

        Pl->speed += (ACCELERATION * dt) ; //increase speed based on acceleration and delta time
        
        if (Pl->speed > 150) Pl->speed = 150; //cap speed to maximum value of 150
        printf("x a before: %d\n", Pl->hitbox.x); // debug info
        
        Pl->hitbox.x -= floor(Pl->speed * dt) ; //update player x position based on speed and delta time
        
        if(center_w && !left_border)
        Props->Camera.x = Pl->hitbox.x - Props->Camera.w / 2; //update camera x position if centered

        printf("x a after: %d\n", Pl->hitbox.x); //
        printf("a: %f\n", Pl->speed * dt);       // debug info
    }

    if (keys[KEY_D] && !keys[KEY_A]) {
        if(Pl->last_direction != 1) Pl->speed = 0; //reset speed if changing direction
        
        Pl->speed += ACCELERATION * dt; //increase speed based on acceleration and delta time
        
        if (Pl->speed > 150) Pl->speed = 150; //cap speed to maximum value of 150
        printf("x d before: %d\n", Pl->hitbox.x); // debug info
        
        Pl->hitbox.x += floor(Pl->speed * dt); //update player x position based on speed and delta time

        if(center_w && !right_border)
        Props->Camera.x = Pl->hitbox.x - Props->Camera.w / 2; //update camera x position if centered

        printf("x d after: %d\n", Pl->hitbox.x); //
        printf("d: %f\n", Pl->speed * dt);       // debug info
    }
    
    if (!keys[KEY_A] && !keys[KEY_D]) {
        Pl->speed = 0; //reset speed if no movement keys are pressed
    } else if (keys[KEY_A] && keys[KEY_D]) {
        Pl->speed = 0; //reset speed if both movement keys are pressed
    }
    if (Pl->hitbox.y >= grnd) Pl->vy = 0; //reset vertical velocity if on ground
    
    if (keys[KEY_SPACE]) {
        printf("space catched\n"); //debug info
        if (Pl->hitbox.y >= grnd) {
            Pl->hitbox.y = grnd; //ensure player is on ground
            Pl->state = lifting; //set jump state to lifting
            Pl->vy = 250.0f; //set initial vertical velocity for jump
            printf("ground check completed\n"); // debug info
        }
    }
    
    if (Pl->state == lifting) jump(Pl, dt, Props, center_h); //handle jumping logic
    if (Pl->hitbox.y <= grnd - JUMP_SCALE) Pl->state = falling; //switch to falling state if maximum jump height is reached
    printf("state processed\n"); // debug info
    
    if (Pl->state == falling && Pl->hitbox.y <= grnd) {
        Pl->vy += 1.0f; //increase vertical velocity to simulate gravity
        if (Pl->vy > 75.0f) Pl->vy = 75.0f; //cap vertical velocity to maximum fall speed
        
        Pl->hitbox.y += ceil(Pl->vy * dt); //update player y position based on vertical velocity and delta time
    
        if(center_h && !bottom_border)
        Props->Camera.y = Pl->hitbox.y - Props->Camera.h / 2; //update camera y position if centered

        printf("falled %f pixels\n", ceil(Pl->vy * dt)); //
        printf("y = %d\n", Pl->hitbox.y);                // debug info
        printf("jump processed\n");                      //
    }
    
    if (keys[KEY_F11]) {
        //printf("Key catched\n");
        if (!fullscreen) {
            SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP); //set fullscreen mode
            //printf("fullscreen set\n");
            fullscreen = true; //update fullscreen state
        } else {
            SDL_SetWindowFullscreen(window, 0); //exit fullscreen mode
            //printf("fullscreen disabled\n");
            fullscreen = false; //update fullscreen state
        }
    }
    return 0;
}
