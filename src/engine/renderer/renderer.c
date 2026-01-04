#include <assert.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "renderer.h" //header file
#include <stdio.h>

SDL_Renderer* renderer; //global renderer variable

SpriteList* SpriteList_Create() {
    SpriteList* sprites = malloc(sizeof(SpriteList)); //allocate memory for SpriteList
    
    return sprites; //return allocated SpriteList
}

SpriteList* SpriteList_Init(SpriteList* sprites, const char *path, SDL_Rect rect) {
    SpriteList* new_sprites = sprites; //initialize new_sprites with input sprites
    
    if(!new_sprites) {
        printf("An error occured during SpriteList memory allocation\n"); //check for null pointer
        return NULL;
    }
    
    new_sprites->Items = malloc(sizeof(Sprite)); //allocate memory for 1 Sprite
    if(!new_sprites->Items) {
        printf("An error occured during Sprite memory allocation\n"); //check for null pointer
        return NULL;
    }
    
    new_sprites->capacity = 1; //set capacity to 1
    new_sprites->Items[sprites->capacity - 1].texture = LoadTexture(path); //load texture from given path
    new_sprites->Items[sprites->capacity - 1].rect = rect; //set rectangle
    return new_sprites; //return initialized SpriteList
}

SpriteList* SpriteList_Add(SpriteList* sprites, const char *path, SDL_Rect rect) {
    SpriteList* new_sprites = sprites; //initialize new_sprites with input sprites
    if(!new_sprites) {
        return NULL; //check for null pointer
    }
    
    int size = (new_sprites->capacity + 1) * sizeof(SpriteList); //calculate new memsize
    
    new_sprites->capacity++; //increase capacity by 1
    
    int pos = new_sprites->capacity; //position of new Sprite
    Sprite* old_arr; //temporary pointer for old array
    
    old_arr = realloc(new_sprites->Items, pos * sizeof(Sprite)); //reallocate memory for new array
    new_sprites->Items = old_arr; //update Items pointer
    if(!new_sprites->Items) {
        printf("An error occured during Sprite memory reallocation\n"); //check for null pointer
        return NULL;
    }
    
    new_sprites->Items[pos - 1].texture = LoadTexture(path); //load texture from given path
    
    new_sprites->Items[pos - 1].rect = rect; //set rectangle
    
    return new_sprites; //return updated SpriteList
}

void SpriteList_Destroy(SpriteList* list) {
    if(!list) //check for null pointer
        return;
        
    if(!list->Items) {
        assert(list->capacity == 0); //ensure capacity is 0 if Items is null
        free(list); //free SpriteList memory
        return;
    }
    
    for(int i = 0; i < list->capacity; i++) //free each texture in the array
        SDL_DestroyTexture(list->Items[i].texture); //free texture memory
        
    free(list->Items); //free Items array memory
    free(list); //free SpriteList memory
}

void InitRenderer(SDL_Window* window, int a, int b) {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); //create renderer with acceleration and vsync
    SDL_RenderSetLogicalSize(renderer, a, b); //set logical size
}

void DrawBackground() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 1); //set draw color to white; RGBA
    SDL_RenderClear(renderer); //clear renderer with draw color
    SDL_RenderPresent(renderer); //present the cleared renderer
}

SDL_Texture* LoadTexture(const char *path) {
    
    SDL_Texture* Loadedtexture = IMG_LoadTexture(renderer, path); //load texture from given path
    return Loadedtexture;
}

int Render(SpriteList* sprites, game_properties *Props) {
    if (!sprites) {
        return 1; //check for null pointer
    }
    
    int size = sprites->capacity; //get number of sprites
    //printf("vars initialized\n");
    SDL_RenderClear(renderer); //clear the renderer
    //printf("renderer cleared\n");
    
    for(int i = 0; i < size; i++) { //render each sprite
        if (sprites->Items[i].texture == NULL) {
            printf("Texture has Null pointer\n"); //check for null texture
            continue;
        }
        SDL_Rect dst = sprites->Items[i].rect; //get destination rectangle
        dst.x -= Props->Camera.x; //adjust x position based on camera
        dst.y -= Props->Camera.y; //adjust y position based on camera

        SDL_RenderCopy(renderer, sprites->Items[i].texture, NULL, &dst); //copy texture to renderer at adjusted position
    }
    //printf("Textures copied\n");
    
    SDL_RenderPresent(renderer); //present the rendered frame
    //printf("frame Presented\n");
    return 0;
}

void SetWindowIcon(SDL_Window* window, const char *path) {
SDL_Surface* icon = IMG_Load(path); //load icon surface from given path

    SDL_SetWindowIcon(window, icon); //set window icon
    SDL_FreeSurface(icon); //free icon surface memory
}
