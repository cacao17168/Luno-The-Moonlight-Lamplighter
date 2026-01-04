#ifndef RENDERER_H_
#define RENDERER_H_
#include "physics.h" //for game_properties structure
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//Sprite structure containing texture and rectangle for rendering

typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
} Sprite;

//SpriteList structure containing dynamic array of sprites and its capacity

typedef struct {
    Sprite* Items;
    int capacity;
} SpriteList;

/*functions for managing SpriteList and rendering*/
//creates and returns an empty SpriteList
SpriteList* SpriteList_Create();

//initializes a Sprite and adds it to the SpriteList; returns updated SpriteList
SpriteList* SpriteList_Init(SpriteList* sprites, const char *path, SDL_Rect rect);

//adds a Sprite to the SpriteList; returns updated SpriteList
SpriteList* SpriteList_Add(SpriteList* sprites, const char *path, SDL_Rect rect);

//frees memory allocated for the SpriteList
void SpriteList_Destroy(SpriteList* list);

//initializes the renderer with given window and logical size

void InitRenderer(SDL_Window* window, int a, int b);

//draws the background based on game properties

void DrawBackground();

//loads a texture from the given file path; returns the loaded texture

SDL_Texture* LoadTexture(const char *path);

//renders all sprites in the SpriteList based on game properties; returns 0 on success

int Render(SpriteList* sprites, game_properties *Props);

//sets the window icon from the given file path

void SetWindowIcon(SDL_Window* window, const char *path);

#endif
