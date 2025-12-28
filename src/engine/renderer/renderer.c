#include <assert.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "renderer.h"
#include <stdio.h>

SDL_Renderer* renderer;

SpriteList* SpriteList_Create() {
    SpriteList* sprites = malloc(sizeof(SpriteList));
    
    return sprites;
}

SpriteList* SpriteList_Init(SpriteList* sprites, const char *path, SDL_Rect rect) {
    SpriteList* new_sprites = sprites;
    
    if(!new_sprites) {
        printf("An error occured during SpriteList memory allocation\n");
        return NULL;
    }
    
    new_sprites->Items = malloc(sizeof(Sprite));
    if(!new_sprites->Items) {
        printf("An error occured during Sprite memory allocation\n");
        return NULL;
    }
    
    new_sprites->capacity = 1;
    new_sprites->Items[sprites->capacity - 1].texture = LoadTexture(path);
    new_sprites->Items[sprites->capacity - 1].rect = rect;
    return new_sprites;
}

SpriteList* SpriteList_Add(SpriteList* sprites, const char *path, SDL_Rect rect) {
    SpriteList* new_sprites = sprites;
    if(!new_sprites) {
        return NULL;
    }
    
    int size = (new_sprites->capacity + 1) * sizeof(SpriteList);
    
    new_sprites->capacity++;
    
    int pos = new_sprites->capacity;
    Sprite* old_arr;
    
    old_arr = realloc(new_sprites->Items, pos * sizeof(Sprite));
    new_sprites->Items = old_arr;
    if(!new_sprites->Items) {
        printf("An error occured during Sprite memory reallocation\n");
        return NULL;
    }
    
    new_sprites->Items[pos - 1].texture = LoadTexture(path);
    
    new_sprites->Items[pos - 1].rect = rect;
    
    return new_sprites;
}

void SpriteList_Destroy(SpriteList* list) {
    if(!list)
        return;
        
    if(!list->Items) {
        assert(list->capacity == 0);
        free(list);
        return;
    }
    
    for(int i = 0; i < list->capacity; i++) 
        SDL_DestroyTexture(list->Items[i].texture);
        
    free(list->Items);
    free(list);
}

void InitRenderer(SDL_Window* window, int a, int b) {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(renderer, a, b);
}

void DrawBackground() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

SDL_Texture* LoadTexture(const char *path) {
    
    SDL_Texture* Loadedtexture = IMG_LoadTexture(renderer, path);
    return Loadedtexture;
}

int Render(SpriteList* sprites, game_properties *Props) {
    if (!sprites) {
        return 1;
    }
    
    int size = sprites->capacity;
    //printf("vars initialized\n");
    SDL_RenderClear(renderer);
    //printf("renderer cleared\n");
    
    for(int i = 0; i < size; i++) {
        if (sprites->Items[i].texture == NULL) {
            printf("Texture has Null pointer\n");
            continue;
        }
        SDL_Rect dst = sprites->Items[i].rect;
        dst.x -= Props->Camera.x;
        dst.y -= Props->Camera.y;

        SDL_RenderCopy(renderer, sprites->Items[i].texture, NULL, &dst);
    }
    //printf("Textures copied\n");
    
    SDL_RenderPresent(renderer);
    //printf("frame Presented\n");
    return 0;
}

void SetWindowIcon(SDL_Window* window, const char *path) {
SDL_Surface* icon = IMG_Load(path);

    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);
}
