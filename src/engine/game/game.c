#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "input.h"
#include "renderer.h"
#include "physics.h"

#define FPS 60
#define ONE_FRAME (1000.0f / FPS)

int main() {

SpriteList* sprites = SpriteList_Create();
    game_properties Game = {0};

    bool is_running = true;

    SDL_Event e;
    SDL_Window* window;
    SDL_Rect plyrdst = {0, 0, 32, 32};
    SDL_Rect Camera = {0, 0, 640, 360};

    keytype pressedkeys[KEY_COUNT] = {0};
    player Player = {0};

    float current_time, previous_time, delta_time;
    float frame_start, frame_end, frame_duration;
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error during SDL initialization: %s\n", SDL_GetError());
        return 1;
    }
    
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        printf("Error during IMG initialization: %s\n", SDL_GetError());
        return 1;
    }
    
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
    
    Player.hitbox = plyrdst;

    Game = game_properties_Init(1024, 512, "assets/images/background.png", Camera);
    
    sprites = SpriteList_Init(sprites, "assets/images/playermodel.png", plyrdst);

    SDL_Rect Backgrnd_rect = Game.Size;
    Backgrnd_rect.y += 84;
    sprites = SpriteList_Add(sprites, Game.BackgroundPath, Backgrnd_rect);
    
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
        
        update(pressedkeys, delta_time, &Player, window, &Game);
        //printf("physics updated\n");
        
        sprites->Items[0].rect.x = Player.hitbox.x;
        sprites->Items[0].rect.y = Player.hitbox.y;
        
        Render(sprites, &Game);
        //printf("rendered\n");
        
        frame_end = SDL_GetTicks();
        frame_duration = frame_end - frame_start;
        
        if ((frame_duration / 1000) < ONE_FRAME) {
            usleep((ONE_FRAME - (frame_duration / 1000)) * 1000);
        }
    }
    
    SDL_DestroyWindow(window);
    SpriteList_Destroy(sprites);
    SDL_Quit();
    
    return 0;
}
