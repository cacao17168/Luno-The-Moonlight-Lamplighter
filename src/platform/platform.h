#ifndef PLATFORM_H_
#define PLATFORM_H_

typedef struct renderer_s renderer_t;

typedef struct window_s window_t;

typedef struct game_time_s game_time_t;

typedef struct app_s app_t;

typedef struct texture_s texture_t;

int app_init(app_t *app, const char *title, int w, int h, int log_w, int log_h);
int app_shutdown(app_t *app);
int app_poll_quit(app_t *app);

float time_tick(game_time_t *time);

int platform_renderer_begin(renderer_t *r);
int platform_renderer_end(renderer_t *r);

texture_t* platform_load_texture(renderer_t *r, const char *path);
void platform_free_texture(texture_t *t);

#endif
