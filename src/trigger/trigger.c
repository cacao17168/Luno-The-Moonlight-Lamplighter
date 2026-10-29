#include "trigger.h"
#include <string.h>

static int entity_matches(const trigger_t *t, const entity_t *e) {
    if(!t || !e) return 0;
    if(t->target_mask && !(t->target_mask & (1u << (unsigned int)e->type))) return 0;
    if(t->target_team >= 0 && e->team != t->target_team) return 0;
    return 1;
}

static int aabb_overlap(float ax, float ay, int aw, int ah,
                        float bx, float by, int bw, int bh)
{
    return ax < bx + (float)bw && ax + (float)aw > bx &&
           ay < by + (float)bh && ay + (float)ah > by;
}

void trigger_init(trigger_system_t *ts) {
    memset(ts, 0, sizeof(*ts));
}

void trigger_clear(trigger_system_t *ts) {
    memset(ts, 0, sizeof(*ts));
}

trigger_t *trigger_add(trigger_system_t *ts, const char *name, trigger_mode_t m, float x, float y, int w, int h) {
    if(!ts || !name) return NULL;

    if(ts->count >= MAX_TRIGGERS) return NULL;

    trigger_t *t = &ts->triggers[ts->count++];
    memset(t, 0, sizeof(*t));

    t->name = strdup(name);
    t->active = 1;
    t->x = x;
    t->y = y;
    t->w = w;
    t->h = h;

    t->mode = m;
    t->target_mask = 0;
    t->target_team = -1;
    return t;
}

void trigger_remove(trigger_system_t *ts, trigger_t *t) {
    if(!ts || !t) return;

    trigger_t *start = &ts->triggers[0];
    trigger_t *end = &ts->triggers[ts->count];

    if(t < start || t > end) return;

    ptrdiff_t idx = t - start;
    for(size_t i = (size_t)idx; i < ts->count - 1; i++) {
        ts->triggers[i] = ts->triggers[i + 1];
    }
    ts->count--;
}

trigger_t *trigger_find(trigger_system_t *ts, const char *name) {
    if(!ts || !name) return NULL;

    for(size_t i = 0; i < ts->count; i++) {
        if(strcmp(ts->triggers[i].name, name) == 0)
            return &ts->triggers[i];
    }
    return NULL;
}

void trigger_set_filter(trigger_t *t, unsigned int type_mask, int target_team) {
    if(!t) return;
    t->target_mask = type_mask;
    t->target_team = target_team;
}

void trigger_set_callbacks(trigger_t *t, trigger_fn on_enter, trigger_fn on_stay, trigger_fn on_exit) {
    if(!t) return;
    t->on_enter = on_enter;
    t->on_stay = on_stay;
    t->on_exit = on_exit;
}

void trigger_reset(trigger_t *t) {
    if(!t) return;

    t->fired = 0;
    t->cooldown_timer = 0.0f;
    memset(t->inside, 0, sizeof(t->inside));
}

void trigger_reset_all(trigger_system_t *ts) {
    if(!ts) return;

    for(size_t i = 0; i < ts->count; i++) {
        trigger_reset(&ts->triggers[i]);
    }
}

void trigger_update(trigger_system_t *ts, world_t *w, float dt) {
    if(!ts || !w) return;

    for(size_t ti = 0; ti < ts->count; ti++) {
        trigger_t *t = &ts->triggers[ti];
        if(!t->active) continue;
        if(t->mode == TRIGGER_ONCE && t->fired) continue;

        if(t->cooldown_timer > 0.0f) {
            t->cooldown_timer -= dt;
            if(t->cooldown_timer < 0.0f) t->cooldown_timer = 0.0f;
        }
        for(size_t i = 0; i < w->count && i < MAX_ENTITIES; i++) {
            entity_t *e = &w->entities[i];
            if(!e->alive) continue;

            int is_in = 0;

            if(entity_matches(t, e)) {
                is_in = aabb_overlap(t->x, t->y, t->w, t->h, e->x, e->y, e->w, e->h);
            }

            int was_in = t->inside[i];

            if(is_in && !was_in) {
                int allow = 1;
                if(t->mode == TRIGGER_ONCE && t->cooldown_timer > 0.0f) allow = 0;

                if(allow) {
                    t->inside[i] = 1;
                    if(t->on_enter) t->on_enter(t, e, w, dt);

                    if(t->mode == TRIGGER_ONCE) {
                        t->fired = 1;
                        break;
                    }

                    if(t->mode == TRIGGER_COOLDOWN) {
                        t->cooldown_timer = t->cooldown;
                    }
                }
            } else if(is_in && was_in) {
                if(t->on_stay) t->on_stay(t, e, w, dt);
            } else if(!is_in && was_in) {
                t->inside[i] = 0;
                if(t->on_exit && e->alive) {
                    t->on_exit(t, e, w, dt);
                }
            }
        }
    }
}
