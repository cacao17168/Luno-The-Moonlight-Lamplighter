#include <signal.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "input.h"
#include "renderer.h"
#include "physics.h"

#define FPS 60
#define ONE_FRAME (1000.0f / FPS)
#define N 2

void sighand(int sig) {
    printf("segmentation fault caught\n");
    SDL_Quit();
    exit(1);
}

int main() {

SpriteList* sprites = SpriteList_Create();

bool is_running = true;

SDL_Event e;
SDL_Window* window;
SDL_Rect plyrdst = {0, 0, 32, 32};
SDL_Rect bckgrnd = {0, 0, 640, 360};

keytype pressedkeys[KEY_COUNT] = {0};
player Player = {0, 300 - 35, 0, falling};

float current_time, previous_time, delta_time;
float frame_start, frame_end, frame_duration;

    signal(SIGSEGV, sighand);
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error during SDL initialization: %s\n", SDL_GetError());
        return 1;
    }
    
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        printf("Error during IMG initialization: %s\n", SDL_GetError());
        return 1;
    }
    fullscreen = false;
    
    window = SDL_CreateWindow("Luno: The Moonlight Lamplighter",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
        SDL_WINDOW_SHOWN);
    
    if (!window) {
        printf("an error %s occured during the creating of the window\n", SDL_GetError());
        return 1;
    }
    
    SetWindowIcon(window, "./assets/images/icon.png");
    
    InitRenderer(window, 640, 360);
    
    sprites = SpriteList_Init(sprites, "assets/images/playermodel.png", plyrdst);
    assert(sprites->Items);
    
    sprites = SpriteList_Add(sprites, "assets/images/background.png", bckgrnd);
    assert(sprites->Items);
    
    DrawBackground();
    
    previous_time = SDL_GetTicks();
    
    while(is_running) {
        frame_start = SDL_GetTicks();
        current_time = SDL_GetTicks();
        
        delta_time = (current_time - previous_time) / 1000.0f;
        previous_time = current_time;
        
        if (delta_time > 0.016f) {
            delta_time = 0.016f;
        }
        
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                is_running = false;
            }
        }
        
        Getkey(pressedkeys);
        
        update(pressedkeys, delta_time, &Player, window);
        //printf("physics updated\n");
        
        sprites->Items[0].rect.x = Player.x;
        sprites->Items[0].rect.y = Player.y;
        
        Render(sprites);
        //printf("rendered\n");
        
        frame_end = SDL_GetTicks();
        frame_duration = frame_end - frame_start;
        
        if ((frame_duration / 1000) < ONE_FRAME) {
            SDL_Delay(ONE_FRAME - (frame_duration / 1000));
        }
    }
    
    SDL_DestroyWindow(window);
    SpriteList_Destroy(sprites);
    SDL_Quit();
    
    return 0;
}
