#ifndef RENDERER_H_
#define RENDERER_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

typedef struct {
    SDL_Texture* texture;
    SDL_Rect rect;
} Sprite;

typedef struct {
    Sprite* Items;
    int capacity;
} SpriteList;

SpriteList* SpriteList_Create();

SpriteList* SpriteList_Init(SpriteList* sprites, const char *path, SDL_Rect rect);

SpriteList* SpriteList_Add(SpriteList* sprites, const char *path, SDL_Rect rect);

void SpriteList_Destroy(SpriteList* list);

void InitRenderer(SDL_Window* window, int a, int b);

void DrawBackground();

SDL_Texture* LoadTexture(const char *path);

int Render(SpriteList* sprites);

void SetWindowIcon(SDL_Window* window, const char *path);

#endif
