#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

map_t* map_load(const char *filename) {
    if(!filename) return NULL;

    FILE* f = fopen(filename, "r");
    if(!f) return NULL;

    int id, w, h, ts;
    fscanf(f, "%d %d %d %d", &id, &w, &h, &ts);

    map_t* map = malloc(sizeof(map_t));
    map->id = id;
    map->width = w;
    map->height = h;
    map->tile_size = ts;
    map->tiles = malloc(sizeof(tile_type) * w * h);

    for(size_t y = 0; y < h; y++) {
        for(size_t x = 0; x < w; x++) {
            int val;
            fscanf(f, "%d", &val);
            map->tiles[y * w + x] = (tile_type)val;
        }
    }
    fclose(f);
    return map;
}

void map_free(map_t *map) {
    if(!map) return;

    free(map->tiles);
    free(map);
    map = NULL;
}

int map_is_solid(map_t *map, int world_x, int world_y) {
    if(!map) return -1;

    int grid_x = ceil((float)world_x / map->tile_size);
    int grid_y = ceil((float)world_y / map->tile_size);

    if(grid_x < 0 || grid_x >= map->width || grid_y < 0 || grid_y >= map->height) {
        return 1;
    }

    return map->tiles[grid_y * map->width + grid_x] == TILE_SOLID;
}

tile_type map_get_tile(const map_t *map, int grid_x, int grid_y) {
    if(grid_x < 0 || grid_x >= map->width || grid_y < 0 || grid_y >= map->height) {
        return TILE_EMPTY;
    }

    return map->tiles[grid_y * map->width + grid_x];
}
