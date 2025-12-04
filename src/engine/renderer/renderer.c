#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "renderer.h"
#include <stdio.h>

SDL_Renderer* renderer;

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

int Render(SDL_Texture* texture[], SDL_Rect *dst[], int size) {
    if (!texture) {
        return 1;
    }
    //printf("vars initialized\n");
    SDL_RenderClear(renderer);
    //printf("renderer cleared\n");
    
    for(int i = 0; i < size; i++) {
        if (texture[i] == NULL) {
            continue;
        }
        SDL_RenderCopy(renderer, texture[i], NULL, dst[i]);
    }
    //printf("Textures copied\n");
    
    SDL_RenderPresent(renderer);
    //printf("frame Presented\n");
    return 0;
}

void SetWindowIcon(SDL_Window* window, char *path) {
SDL_Surface* icon = IMG_Load(path);

    SDL_SetWindowIcon(window, icon);
    SDL_FreeSurface(icon);
}
