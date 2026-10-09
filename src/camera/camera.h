#ifndef CAMERA_H_
#define CAMERA_H_

typedef struct {
    float x;
    float y;
    int width;
    int height;
} camera_t;

int camera_update(camera_t *camera, float x, float y, int map_width, int map_height);

#endif
