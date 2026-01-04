#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "input.h" // my input module
#include "renderer.h" //module with rendering functions
#include "physics.h" //module with "update()" function

#define FPS 60
#define ONE_FRAME (1000.0f / FPS)

int main() {

/*look header files before learning main()*/

SpriteList* sprites = SpriteList_Create();
    game_properties Game = {0};

    bool is_running = true;

    SDL_Event e;
    SDL_Window* window;
    SDL_Rect plyrdst = {0, 0, 32, 32}; //player rectangle (hitbox)
    SDL_Rect Camera = {0, 0, 640, 360}; //camera rectangle

    keytype pressedkeys[KEY_COUNT] = {0}; //array with pressed keys
    player Player = {0}; //player structure, for more info see "physics.h"

    float current_time, previous_time, delta_time; //time variables for delta time calculation
    float frame_start, frame_end, frame_duration; //time variables for fps limiting

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
    
    SetWindowIcon(window, "./assets/images/icon.png"); //set window icon
    
    InitRenderer(window, 640, 360); //initialize renderer with logical size 640x360
    
    Player.hitbox = plyrdst; //set player hitbox size

    Game = game_properties_Init(1024, 512, "assets/images/testbackgrnd.png", Camera); //initialize game properties
    
    sprites = SpriteList_Init(sprites, "assets/images/playermodel.png", plyrdst); //initialize sprite list with player sprite

    SDL_Rect Backgrnd_rect = Game.Size; //copy background size
    Backgrnd_rect.y += 111; //offset background by 111 pixels for better visual effect
    sprites = SpriteList_Add(sprites, Game.BackgroundPath, Backgrnd_rect); //add background sprite to sprite list
    
    DrawBackground(); //draw background once to prevent black screen on start
    
    previous_time = SDL_GetTicks();
    
    while(is_running) {
        frame_start = SDL_GetTicks();
        current_time = SDL_GetTicks();
        
        delta_time = (current_time - previous_time) / 1000.0f; //calculate delta time in seconds
        previous_time = current_time;
        
        if (delta_time > 0.016f) {
            delta_time = 0.016f; //cap delta time to 16ms to prevent big jumps
        }
        
        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                is_running = false; //handle window close event
            }
        }
        
        Getkey(pressedkeys); //catch pressed keys
        
        update(pressedkeys, delta_time, &Player, window, &Game); //update physics
        //printf("physics updated\n"); this line is for debugging, will delete later
        
        sprites->Items[0].rect.x = Player.hitbox.x; //update player sprite x position for rendering
        sprites->Items[0].rect.y = Player.hitbox.y; //update player sprite y position for rendering
        
        Render(sprites, &Game); //render all sprites
        //printf("rendered\n"); this line is for debugging, will delete later
        
        frame_end = SDL_GetTicks();
        frame_duration = frame_end - frame_start;
        
        if ((frame_duration / 1000) < ONE_FRAME) {
            usleep((ONE_FRAME - (frame_duration / 1000)) * 1000); //limit fps to defined value
        }
    }
    
    SDL_DestroyWindow(window); //destroy window
    SpriteList_Destroy(sprites); //destroy sprite list and free memory
    SDL_Quit(); //quit SDL
    
    return 0;
}
