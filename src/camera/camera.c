#include "camera.h"

int isLeft_border(float x) {
    if(x <= 0) return 1;
    return 0;
}

int isRight_border(float x, int cam_w, int map_w) {
    if(x + cam_w >= map_w) return 1;
    return 0;
}

int isTop_border(float y) {
    if(y <= 0) return 1;
    return 0;
}

int isBottom_border(float y, int cam_h, int map_h) {
    if(y + cam_h >= map_h) return 1;
    return 0;
}

int camera_update(camera_t *camera, float x, float y, int map_width, int map_height) {
    if(!camera) return 1;

    if(!isLeft_border(x)) {
        if(!isRight_border(x, camera->width, map_width)) {
            camera->x = x;
        } else {
            camera->x = map_width;
        }
    } else {
        camera->x = 0;
    }

    if(!isTop_border(y)) {
        if(!isBottom_border(y, camera->height, map_height)) {
            camera->y = y;
        } else {
            camera->y = map_height;
        }
    } else {
        camera->y = 0;
    }
    return 0;
}
