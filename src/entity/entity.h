#ifndef ENTITY_H_
#define ENTITY_H_
#include "../animations/animation.h"
#include "../input/input.h"

#define MAX_ENTITIES 256

typedef enum {
    ENTITY_NONE = 0,
    ENTITY_PLAYER,
    ENTITY_ENEMY,
    ENTITY_PROJECTILE
} entity_type;

typedef struct entity_s entity_t;
typedef struct world_s world_t;

typedef void (*entity_update_fn)(entity_t *self, world_t *w, float dt);
typedef void (*entity_touch_fn)(entity_t *self, entity_t *other, world_t *w);

struct entity_s {
    entity_type type;
    int alive; //0 on false, 1 on true

    float x, y;
    float vx, vy;
    int w, h;

    int health;
    int damage;
    int team; //0 - players, 1 - enemies, teammates don't hit each other

    float lifetime;
    float touch_cooldown;

    anim_state_t anim;
    int facing;

    entity_update_fn update;
    entity_touch_fn on_touch;

    int on_ground; //0 on false, 1 on true
};

struct world_s {
    entity_t entities[MAX_ENTITIES];
    int count;

    const kb_key_t *input;
};

void world_init(world_t *w);
entity_t *world_spawn(world_t *w);
entity_t *world_player(world_t *w);

entity_t *spawn_player(world_t *w, float x, float y);
entity_t *spawn_enemy(world_t *w, float x, float y);
entity_t *spawn_projectile(world_t *w, float x, float y, float vx, float vy, int team, int damage);

void world_update(world_t *w, kb_key_t *in, float dt);

#endif
