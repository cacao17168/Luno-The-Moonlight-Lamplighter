#ifndef TRIGGER_H_
#define TRIGGER_H_
#include "../entity/entity.h"
#include <stddef.h>
#define MAX_TRIGGERS 128

typedef enum {
    TRIGGER_ONCE = 0,
    TRIGGER_REPEAT,
    TRIGGER_COOLDOWN
} trigger_mode_t;

typedef struct trigger_s trigger_t;

typedef void (*trigger_fn)(trigger_t *self, entity_t *activator, world_t *w, float dt);

struct trigger_s {
    const char *name;
    int active; //0 on false, 1 on true
    int fired; //0 on false, 1 on true

    float x, y;
    int w, h;

    unsigned int target_mask; //0 for all or 1u << entity->type, bitmask
    int target_team; //-1 for all or coincidence with entity->type

    trigger_mode_t mode;
    float cooldown;
    float cooldown_timer;

    unsigned char inside[MAX_ENTITIES];
    trigger_fn on_enter;
    trigger_fn on_stay;
    trigger_fn on_exit;
};

typedef struct {
    trigger_t triggers[MAX_TRIGGERS];
    size_t count;
} trigger_system_t;

void trigger_init(trigger_system_t *ts);
void trigger_clear(trigger_system_t *ts);

trigger_t *trigger_add(trigger_system_t *ts, const char *name, trigger_mode_t m, float x, float y, int w, int h);
void trigger_remove(trigger_system_t *ts, trigger_t *t);
trigger_t *trigger_find(trigger_system_t *ts, const char *name);

void trigger_set_filter(trigger_t *t, unsigned int type_mask, int target_team);
void trigger_set_callbacks(trigger_t *t, trigger_fn on_enter, trigger_fn on_stay, trigger_fn on_exit);

void trigger_reset(trigger_t *t);
void trigger_reset_all(trigger_system_t *ts);

void trigger_update(trigger_system_t *ts, world_t *w, float dt);

#endif
