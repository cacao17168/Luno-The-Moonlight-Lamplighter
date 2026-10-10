#ifndef MAP_H_
#define MAP_H_

typedef enum {
    TILE_EMPTY = 0,
    TILE_SOLID = 1,
    TILE_PLATFORM = 2
} tile_type;

typedef struct {
    int id;
    int width;
    int height;
    int tile_size;
    tile_type *tiles;
} map_t;

map_t* map_load(const char *filename);

void map_free(map_t *map);

#endif
