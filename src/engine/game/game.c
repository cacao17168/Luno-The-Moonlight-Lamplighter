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
    abort();
}

int main() {
bool is_running = true;
SDL_Event e;
SDL_Window* window;
keytype pressedkeys[KEY_COUNT] = {0};
player Player = {0, 300 - 35, 0, falling};

SDL_Rect *rects[N];
    for(int i = 0; i < N; i++) {
        rects[i] = malloc(sizeof(SDL_Rect));
        if (!rects[i]) {
            printf("memory alloc failed\n");
            break;
        }
    }

SDL_Texture* textures[N];

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
    
    DrawBackground();
    
    textures[0] = LoadTexture("./assets/images/playermodel.png");
    assert(textures[0]);
    
    rects[0]->x = 0;
    rects[0]->y = 0;
    rects[0]->w = 32;
    rects[0]->h = 32;
    
    textures[1] = LoadTexture("./assets/images/background.png");
    assert(textures[1]);
    
    rects[1]->x = 0;
    rects[1]->y = 0;
    rects[1]->w = 640;
    rects[1]->h = 360;
    
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
        
        rects[0]->x = Player.x;
        rects[0]->y = Player.y;
        
        Render(textures, rects, N);
        //printf("rendered\n");
        
        frame_end = SDL_GetTicks();
        frame_duration = frame_end - frame_start;
        
        if ((frame_duration / 1000) < ONE_FRAME) {
            SDL_Delay(ONE_FRAME - (frame_duration / 1000));
        }
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    for(int i = 0; i < N; i++) {
        free(rects[i]);
    }
    
    return 0;
}
