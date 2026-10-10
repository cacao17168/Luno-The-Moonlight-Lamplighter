#include "platform.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

int app_init(app_t *app, const char *title, int w, int h, int log_w, int log_h) {
    if(!app || !title) return 1;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) return 2;
    IMG_Init(IMG_INIT_PNG);

    app->window.height = h;
    app->window.width = w;
    app->window.title = title;
    app->window.sdl_wnd = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, SDL_WINDOW_SHOWN);

    app->renderer.window_link = app->window.sdl_wnd;
    app->renderer.logic_w = log_w;
    app->renderer.logic_h = log_h;
    app->renderer.sdl = SDL_CreateRenderer(app->renderer.window_link, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_RenderSetLogicalSize(app->renderer.sdl, log_w, log_h);
    return 0;
}

int app_shutdown(app_t *app) {
    if(!app) return 1;

    SDL_DestroyRenderer(app->renderer.sdl);
    SDL_DestroyWindow(app->window.sdl_wnd);
    SDL_Quit();
    return 0;
}

int app_poll_quit(app_t *app) {
    SDL_Event e;
    if(SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_QUIT, SDL_QUIT) > 0) return 1;
    return 0;
}

float time_tick(game_time_t *time) {
    if(!time) return -1;

    time->prev_ticks = time->cur_ticks;
    time->cur_ticks = SDL_GetPerformanceCounter();
    time->freq = SDL_GetPerformanceFrequency();

    time->total_pass += (double)(time->cur_ticks - time->prev_ticks) / time->freq;
    return (float)(time->cur_ticks - time->prev_ticks) / time->freq;
}

int renderer_begin(renderer_t *r) {
    if(!r) return 1;

    SDL_RenderClear(r->sdl);
    return 0;
}

int renderer_end(renderer_t *r) {
    if(!r) return 1;

    SDL_RenderPresent(r->sdl);
    return 0;
}

texture_t* load_texture(renderer_t *r, const char *path) {
    if(!r || !path) return NULL;
    texture_t *t = malloc(sizeof(texture_t));
    t->path = path;
    t->surf = IMG_Load(t->path);
    t->texture = SDL_CreateTextureFromSurface(r->sdl, t->surf);
    return t;
}
