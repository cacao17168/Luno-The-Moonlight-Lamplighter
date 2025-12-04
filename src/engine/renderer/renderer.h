#ifndef RENDERER_H_
#define RENDERER_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

//SDL_Texture* Textures;

void InitRenderer(SDL_Window* window, int a, int b);

void DrawBackground();

SDL_Texture* LoadTexture(const char *path);

int Render(SDL_Texture* texture[], SDL_Rect *dst[], int size);

void SetWindowIcon(SDL_Window* window, char *path);

#endif
