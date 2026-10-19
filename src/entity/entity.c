#include "entity.h"
#include <string.h>

static const anim_clip_t clip;

void world_init(world_t *w) {
    memset(w, 0, sizeof(*w));
    w->count = 0;
    w->input = NULL;
}

entity_t *world_spawn(world_t *w) {
    if(!w) return NULL;
    if(w->count >= MAX_ENTITIES) return NULL;

    entity_t *e = &w->entities[w->count++];
    memset(e, 0, sizeof(*e));

    e->alive = 1;
    e->team = 1;
    e->lifetime = -1.0f;
    e->facing = 1;
    return e;
}

entity_t *world_player(world_t *w) {
    for(size_t i = 0; i < w->count; i++) {
        if(w->entities[i].type == ENTITY_PLAYER && w->entities[i].alive) {
            return &w->entities[i];
        }
    }
    return NULL;
}

entity_t *spawn_player(world_t *w, float x, float y) {
    if(!w) return NULL;
    if(w->count >= MAX_ENTITIES) return NULL;

    entity_t *e = &w->entities[w->count++];
    memset(e, 0, sizeof(*e));

    e->type = ENTITY_PLAYER;
    e->alive = 1;
    e->x = x;
    e->y = y;
    e->vx = 0;
    e->vy = 0;
    e->w = 32;
    e->h = 32;
    e->health = 6;
    e->damage = 2;
    e->team = 0;
    e->update = NULL;   //while there is no implementation of these funtions there will be null pointers
    e->on_touch = NULL; //

    return e;
}
