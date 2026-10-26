#include "platform.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef struct renderer_s {
    SDL_Renderer *sdl;
    SDL_Window *window_link;
    int logic_h;
    int logic_w;
} renderer_t;

typedef struct window_s {
    SDL_Window *sdl_wnd;
    int height;
    int width;
    const char *title;
} window_t;

typedef struct color_s {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} color_t;

typedef struct font_s {
    TTF_Font* font;
    const char *ttf_file;
} font_t;

typedef struct game_time_s {
    Uint64 prev_ticks;
    Uint64 cur_ticks;
    Uint64 freq;
    double total_pass;
} game_time_t;

typedef struct app_s {
    window_t window;
    renderer_t renderer;
    game_time_t time;
    font_t main_font;
    int running; //1 when running and 0 when not running
} app_t;

typedef struct texture_s {
    const char *path;
    SDL_Surface *surf;
    SDL_Texture *texture;
} texture_t;

int app_init(app_t *app, const char *title, int w, int h, int log_w, int log_h) {
    if(!app || !title) return 1;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) return 2;
    if(IMG_Init(IMG_INIT_PNG) <= 0) return 3;
    if(TTF_Init() < 0) return 4;

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
    IMG_Quit();
    TTF_Quit();
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

int platform_renderer_begin(renderer_t *r) {
    if(!r) return 1;

    SDL_RenderClear(r->sdl);
    return 0;
}

int platform_renderer_end(renderer_t *r) {
    if(!r) return 1;

    SDL_RenderPresent(r->sdl);
    return 0;
}

texture_t* platform_load_texture(renderer_t *r, const char *path) {
    if(!r || !path) return NULL;
    texture_t *t = malloc(sizeof(texture_t));
    t->path = path;
    t->surf = IMG_Load(t->path);
    t->texture = SDL_CreateTextureFromSurface(r->sdl, t->surf);
    return t;
}

void platform_free_texture(texture_t *t) {
    if(!t) return;

    SDL_FreeSurface(t->surf);
    SDL_DestroyTexture(t->texture);
    free(t);
}

font_t *platform_load_ttf(char *filename) {
    if(!filename) return NULL;
    TTF_Font* ttf_font = TTF_OpenFont(filename, 8);
    if(!ttf_font) return NULL;
    font_t *font = malloc(sizeof(font_t));
    font->font = ttf_font;
    font->ttf_file = filename;
    return font;
}

void platform_free_ttf(font_t *f) {
    if(!f) return;
    TTF_CloseFont(f->font);
    free(f);
    f = NULL;
}
