#include "animation.h"

void anim_play(anim_state_t *a, const anim_clip_t *clip) {
    if(!a || !clip) return;
    if(a->clip == clip) return;

    a->clip = clip;
    a->current_frame = 0;
    a->timer = 0;
    a->finished = 0;
}

void anim_update(anim_state_t *a, float dt) {
    if(!a || a->finished) return;
    if(a->clip->frame_count <= 1) return;

    a->timer += dt;

    while(a->timer >= a->clip->frame_time) {
        a->timer -= a->clip->frame_time;
        a->current_frame++;

        if(a->current_frame >= a->clip->frame_count) {
            if(a->clip->loop) {
                a->current_frame = 0;
            } else {
                a->current_frame = a->clip->frame_count - 1;
                a->finished = 1;
                break;
            }
        }
    }
}

int anim_cur_frame(const anim_state_t *a) {
    if(!a->clip) return -1;
    return a->clip->start_frame + a->current_frame;
}
