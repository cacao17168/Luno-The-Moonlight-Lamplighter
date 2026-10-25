#include "entity.h"
#include <string.h>
#include <math.h>

#define PLAYER_ACCEL 1800.0f
#define PLAYER_MAX_SPEED 230.0f
#define PLAYER_FRICTION 1600.0f
#define PLAYER_JUMP_VEL -540.0f
#define PLAYER_GRAVITY 1500.0f
#define PLAYER_MAX_FALL 900.0f

#define PLAYER_FIRE_CD 0.18f
#define PLAYER_TOUCH_CD 0.75f
#define PLAYER_SHOT_SPEED 520.0f

#define ENEMY_SPEED 110.0f
#define ENEMY_GRAVITY 1500.0f
#define ENEMY_MAX_FALL 900.0f
#define ENEMY_FIRE_CD 1.0f
#define ENEMY_TOUCH_CD 0.75f
#define ENEMY_SHOT_SPEED 380.0f
#define ENEMY_AGGRO_RANGE 280.0f

static const anim_clip_t clip;

static int rect_hits_solid(map_t *m, float x, float y, int w, int h) {
    if(!m) return -1;

    int ts = m->tile_size;
    int gx0 = (int)floorf(x / ts);
    int gx1 = (int)floorf((x + w - 1) / ts);
    int gy0 = (int)floorf(y / ts);
    int gy1 = (int)floorf((y + h - 1) / ts);

    for(int gy = gy0; gy <= gy1; gy++) {
        for(int gx = gx0; gx <= gx1; gx++) {
            if(map_is_solid(m, gx, gy) == TILE_SOLID) return 1;
        }
    }
    return 0;
}

static void resolve_x(entity_t *e, map_t *m) {
    if(!e || !m) return;

    int ts = m->tile_size;
    float x = e->x;
    float y = e->y;

    int gx0 = (int)floorf(x / ts);
    int gx1 = (int)floorf((x + e->w - 1) / ts);
    int gy0 = (int)floorf(y / ts);
    int gy1 = (int)floorf((y + e->h - 1) / ts);

    for(int gy = gy0; gy <= gy1; gy++) {
        for(int gx = gx0; gx <= gx1; gx++) {
            if(map_get_tile(m, gx, gy) != TILE_SOLID) continue;
            if(e->vx > 0.0f) e->x = (float)(gx * ts) - (float)e->w;
            else e->x = (float)(gx + 1) * ts;
            e->vx = 0.0f;
            return;
        }
    }
}

static void resolve_y(entity_t *e, map_t *m, float prev_bottom) {
    if(!e || !m) return;

    int ts = m->tile_size;
    float x = e->x;
    float y = e->y;

    int gx0 = (int)floorf(x / ts);
    int gx1 = (int)floorf((x + e->w - 1) / ts);
    int gy0 = (int)floorf(y / ts);
    int gy1 = (int)floorf((y + e->h - 1) / ts);

    for(int gy = gy0; gy <= gy1; gy++) {
        for(int gx = gx0; gx <= gx1; gx++) {
            tile_type t = map_get_tile(m, gx, gy);
            int solid = (t == TILE_SOLID);
            int platform = (t == TILE_PLATFORM);
            if(!solid && !platform) continue;

            if(e->vy > 0.0f) {
                if(platform) {
                    float top = (float)(gy * ts);
                    if(prev_bottom > top + 0.5f) continue;
                }
                e->y = (float)(gy * ts) - (float)e->h;
                e->vy = 0.0f;
                e->on_ground = 1;
                return;
            } else if(e->vy < 0.0f && solid) {
                e->y = (float)((gy + 1) * ts);
                e->vy = 0.0f;
                return;
            }
        }
    }
}

static int platform_below(map_t *m, float gx, float gy) {
    if(!m) return -1;
    tile_type t = map_get_tile(m, gx, gy);
    return (t == TILE_PLATFORM);
}

static void player_update(entity_t *self, world_t *w, float dt) {
    if(!self || !w || dt <= 0.0f) return;
    if(!self->alive) return;

    const kb_key_t *in = w->input;
    int left = in[KEY_A];
    int right = in[KEY_D];
    int jump = in[KEY_SPACE];
    int shoot = in[KEY_MOUSE_LMB];

    self->facing = (right - left) > 0 ? 1 : -1;
    if(self->facing != 0) {
        self->vx += self->facing * PLAYER_ACCEL * dt;
        if(self->vx > PLAYER_MAX_SPEED) self->vx = PLAYER_MAX_SPEED;
        if(self->vx < -PLAYER_MAX_SPEED) self->vx = -PLAYER_MAX_SPEED;
    } else {
        float fr = PLAYER_FRICTION * dt;
        if(self->vx > 0.0f) {
            self->vx -= fr;
            if(self->vx < 0.0f) self->vx = 0.0f;
        } else if (self->vx < 0.0f) {
            self->vx += fr;
            if(self->vx > 0.0f) self->vx = 0.0f;
        }
    }

    if(!self->on_ground) {
        self->vy += PLAYER_GRAVITY * dt;
        if(self->vx > PLAYER_MAX_FALL) self->vy = PLAYER_MAX_FALL;
    }

    if(jump & self->on_ground) {
        self->vy = PLAYER_JUMP_VEL;
        self->on_ground = 0;
    }

    self->x = self->vx * dt;
    resolve_x(self, w->map);

    float prev_bottom = self->y + (float)self->h;
    self->y = self->vy * dt;
    resolve_y(self, w->map, prev_bottom);

    if(self->touch_cooldown > 0.0f) {
        self->touch_cooldown -= dt;
        if(self->touch_cooldown < 0.0f) self->touch_cooldown = 0.0f;
    }
    if(self->fire_cooldown > 0.0f) {
        self->fire_cooldown -= dt;
        if(self->fire_cooldown < 0.0f) self->touch_cooldown = 0.0f;
    }

    if(shoot && self->fire_cooldown <= 0.0f) {
        float muzzle_x = self->x + self->w * 0.5f + (float)self->facing * (self->w * 0.5f + 4.0f);
        float muzzle_y = self->y + self->h * 0.4f;
        spawn_projectile(w, muzzle_x, muzzle_y, (float)self->facing * PLAYER_SHOT_SPEED, 0.0f, self->team, self->damage);
        self->fire_cooldown = PLAYER_FIRE_CD;
    }
}

static void player_on_touch(entity_t *self, entity_t *other, world_t *w) {
    (void)w;
    if(!self || !other) return;
    if(!self->alive || !other->alive) return;

    if(other->team == self->team) return;

    if(self->touch_cooldown > 0.0f) return;

    if(other->damage > 0) {
        self->health -= other->damage;
        self->touch_cooldown = PLAYER_TOUCH_CD;
    }

    if(self->health <= 0) {
        self->health = 0;
        self->alive = 0;
    }
}

static void enemy_update(entity_t *self, world_t *w, float dt) {
    if(!self || !w || dt < 0.0f) return;
    if(!self->alive) return;

    if(self->touch_cooldown > 0.0f) {
        self->touch_cooldown -= dt;
        if(self->touch_cooldown < 0.0f) self->touch_cooldown = 0.0f;
    }
    if(self->fire_cooldown > 0.0f) {
        self->fire_cooldown -= dt;
        if(self->fire_cooldown < 0.0f) self->touch_cooldown = 0.0f;
    }

    self->vx = (float)self->facing * ENEMY_SPEED;
    self->vy += ENEMY_GRAVITY * dt;
    if(self->vy > ENEMY_MAX_FALL) self->vy = ENEMY_MAX_FALL;

    float vx_before = self->vx;
    self->x += self->vx * dt;
    resolve_x(self, w->map);
    if(vx_before != 0.0f && self->vx == 0.0f) {
        self->facing = -self->facing;
    }

    if(self->on_ground) {
        float probe_x = self->x + (self->facing > 0 ? (float)self->w : -1.0f);
        float probe_y = self->y + (float)self->h;
        int has_ground = rect_hits_solid(w->map, probe_x, probe_y, 1, 1) || platform_below(w->map, probe_x, probe_y);
        if(!has_ground) self->facing = -self->facing;
    }

    float prev_bottom = self->y + (float)self->h;
    self->y += self->vy * dt;
    resolve_y(self, w->map, prev_bottom);

    if(self->fire_cooldown <= 0.0f) {
        entity_t *p = world_player(w);
        if(p && p->alive) {
            float dx = p->x - self->x;
            float dy = p->y - self->y;
            if(fabsf(dx) < ENEMY_AGGRO_RANGE && fabsf(dy) < 80.0f) {
                self->facing = (dx > 0.0f) ? 1 : -1;
                float mx = self->x + self->w * 0.5f + (float)self->facing * (self->w * 0.5f + 4.0f);
                float my = self->y + self->h * 0.4f;
                spawn_projectile(w, mx, my, (float)self->facing * ENEMY_SHOT_SPEED, 0.0f, self->team, self->damage);
                self->fire_cooldown = ENEMY_FIRE_CD;
            }
        }
    }
}

static void enemy_on_touch(entity_t *self, entity_t *other, world_t *w) {
    (void)w;
    if(!self || !other) return;
    if(!self->alive || !other->alive) return;

    if(other->team == self->team) return;

    if(self->touch_cooldown > 0.0f) return;

    if(other->damage > 0) {
        self->health -= other->damage;
        self->touch_cooldown = ENEMY_TOUCH_CD;
    }

    if(self->health <= 0) {
        self->health = 0;
        self->alive = 0;
    }
}

static void projectile_on_touch(entity_t *self, entity_t *other, world_t *w) {
    (void)w;
    if(!self || !other) return;
    if(!self->alive || !other->alive) return;

    if(other->team == self->team) return;

    self->health = 0;
    self->alive = 0;
}

void world_init(world_t *w) {
    memset(w, 0, sizeof(*w));
    w->count = 0;
    w->input = NULL;
    w->map = NULL; //need to be initialized in main.c now, fix in future
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
    e->update = player_update;
    e->on_touch = player_on_touch;

    return e;
}
