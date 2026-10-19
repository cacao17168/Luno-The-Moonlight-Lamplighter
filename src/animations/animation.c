#include "animation.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static anim_clip_t parse_anim_line(char *line) {
    anim_clip_t clip = {
        .t_paths = NULL,
        .frame_count = 0,
        .frame_time = 0,
        .loop = 0
    };
    const char *delims = ",\n";
    if(!line) return clip;

    char *line_copy = strdup(line);
    char *running = line_copy;
    size_t token_count = 0;

    while(strsep(&running, delims) != NULL) {
        token_count++;
    }
    free(line_copy);

    if(token_count < 4) return clip;

    size_t path_count = token_count - 3;

    clip.t_paths = malloc(path_count * sizeof(char *));
    clip.frame_count = path_count;

    running = line;

    for(size_t i = 0; i < path_count; i++) {
        char *token = strsep(&running, delims);
        if(token != NULL && strlen(token) > 0) {
            clip.t_paths[i] = strdup(token);
        }
    }

    char *token;

    if ((token = strsep(&running, delims)) != NULL && strlen(token) > 0) {
        int read_count = (int)strtol(token, NULL, 10);
        if(read_count > clip.frame_count) clip.frame_count = read_count;
    }

    if ((token = strsep(&running, delims)) != NULL && strlen(token) > 0) {
        clip.frame_time = strtod(token, NULL);
    }

    if ((token = strsep(&running, delims)) != NULL && strlen(token) > 0) {
        clip.loop = (strtol(token, NULL, 10) == 0) ? 0 : 1;
    }

    return clip;
}

anim_clip_t *anim_cfg_load(const char *filename) {
    if(!filename) return NULL;
    FILE *f = fopen(filename, "r");
    if(!f) {
        return NULL;
    }

    anim_clip_t *clips = NULL;
    size_t count = 0;
    char line[1024];

    while(fgets(line, sizeof(line), f)) {
        if(line[0] == '\n' || line[0] == '\r' || line[0] == '#') {
            continue;
        }

        anim_clip_t clip = parse_anim_line(line);

        if(clip.t_paths != NULL) {
            count++;
            anim_clip_t *temp = realloc(clips, sizeof(anim_clip_t) * count);
            if(!temp) {
                fclose(f);
                return NULL;
            }
            clips = temp;
            clips[count - 1] = clip;
        }
    }
    fclose(f);

    if(clips != NULL) {
        count++;
        anim_clip_t *temp = realloc(clips, sizeof(anim_clip_t) * count);
        if(temp) {
            clips = temp;
            clips[count - 1] = (anim_clip_t){.t_paths = NULL, .frame_count = 0};
        }
    }

    return clips;
}

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
    if(a->clip->frame_count <= 1) {
        if(!a->clip->loop) a->finished = 1;
        return;
    }

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
    if(!a) return -1;
    return a->current_frame;
}
