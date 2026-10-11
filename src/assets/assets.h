#ifndef ASSETS_H_
#define ASSETS_H_
#include "../platform/platform.h"

typedef struct {
    texture_t *texture;
    int width;
    int height;
} asset_t;

void assets_init(void);

texture_t* assets_get_texture(renderer_t *r, const char *path);

int assets_has_texture(const char *path);

int assets_unload_texture(const char *path);

void assets_shutdown(void);

#endif
