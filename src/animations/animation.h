#ifndef ANIMATION_H_
#define ANIMATION_H_

/* anim_clip_t adapted for sprite animation via individual files.
 * This reduces perfomance and big projects with more than 1000 sprites should use spritelists,
 * but in small games spritelists will give a little impact,
 * so using individual textures is more convenient
 */
typedef struct {
    char **t_paths;
    int frame_count;
    float frame_time;
    int loop; //0 on false, 1 on true
} anim_clip_t;

typedef struct {
    const anim_clip_t *clip;
    int current_frame;
    float timer;
    int finished; //0 on false, 1 on true
} anim_state_t;

void anim_play(anim_state_t *a, const anim_clip_t *clip);
void anim_update(anim_state_t *a, float dt);

int anim_cur_frame(const anim_state_t *a);

#endif
