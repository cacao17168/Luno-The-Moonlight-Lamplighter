#ifndef PLATFORM_H_
#define PLATFORM_H_
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>

typedef struct {
    SDL_Renderer *sdl;
    SDL_Window *window_link;
    int logic_h;
    int logic_w;
} renderer_t;

typedef struct {
    SDL_Window *sdl_wnd;
    int height;
    int width;
    const char *title;
} window_t;

typedef struct {
    Uint64 prev_ticks;
    Uint64 cur_ticks;
    Uint64 freq;
    double total_pass;
} game_time_t;

typedef struct {
    window_t window;
    renderer_t renderer;
    game_time_t time;
    int running; //1 when running and 0 when not running
} app_t;

typedef struct {
    const char *path;
    SDL_Surface *surf;
    SDL_Texture *texture;
} texture_t;

int app_init(app_t *app, const char *title, int w, int h, int log_w, int log_h);
int app_shutdown(app_t *app);
int app_poll_quit(app_t *app);

float time_tick(game_time_t *time);

int renderer_begin(renderer_t *r);
int renderer_end(renderer_t *r);

texture_t* load_texture(const char *path);

#endif
