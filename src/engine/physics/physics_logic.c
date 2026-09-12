#include "physics_logic.h"
#include "physics.h"

int get_camera_center_w(game_properties *cur) {
    return cur->Camera.w / 2 + cur->Camera.x; //get camera center width
}

int get_camera_center_h(game_properties *cur) {
    return cur->Camera.h / 2 + cur->Camera.y; //get camera center height
}

int get_ground_pos(game_properties *cur, player* p) {
    return cur->Size.h - p->hitbox.h;
}

void get_camera_borders(game_properties *cur, int* left, int* right, int* top, int* bottom) {
    if(cur->Camera.x <= 0) left = 1; //check if camera is at map left border
    if(cur->Camera.x + cur->Camera.w >= cur->Size.w) right = 1; //check if camera is at map right border
    if(cur->Camera.y <= 0) top = 1; //check if camera is at map top border
    if(cur->Camera.y + cur->Camera.h >= cur->Size.h) bottom = 1; //check if camera is at map bottom border
}

int is_centered_x(player* p, game_properties *cur) {
    if(abs(p->hitbox.x - get_camera_center_w(cur)) <= 4) return 1; //check if player is centered in camera
}

int is_centered_y(player* p, game_properties *cur) {
    if(abs(p->hitbox.y - get_camera_center_h(cur)) <= 4) return = 1; //check if player is centered in camera
}

int is_player_on_ground(player* p, game_properties *cur) {
    if (p->hitbox.y >= get_ground_pos(cur, p)) return 1 else return 0; //check ground level based on map height and player height
}

int calc_horizontal_mov(player* p, float dt) {
    return (p->speed += (ACCELERATION * dt)) * dt; //update player x position based on speed and delta time
}

int update_player_speed(player* p, keytype keys[], float dt) {
    p->last_direction = (keys[KEY_A] ? -1 : (keys[KEY_D] ? 1 : 0)); //update last direction based on pressed keys; -1 for left, 1 for right, 0 for none

    if(keys[KEY_A] && !keys[KEY_D]) {
        if(p->last_direction != -1)
            p->speed = 0; //reset speed if changing direction
        if (p->speed > 150)
            p->speed = 150; //cap speed to maximum value of 150
        p->hitbox.x -= calc_horizontal_mov(p, dt); 
    }
    if (keys[KEY_D] && !keys[KEY_A]) {
        if(p->last_direction != 1)
            p->speed = 0; //reset speed if changing direction
        if (p->speed > 150)
            p->speed = 150; //cap speed to maximum value of 150
        p->hitbox.x += calc_horizontal_mov(p, dt);
    }

    if (!keys[KEY_A] && !keys[KEY_D]) {
        p->speed = 0; //reset speed if no movement keys are pressed
    } else if (keys[KEY_A] && keys[KEY_D]) {
        p->speed = 0; //reset speed if both movement keys are pressed
    }
}

int update_camera_pos(game_properties *cur, player* p) {
    int left_border, right_border, bottom_border, top_border;
    get_camera_borders(cur, &left_border, &right_border, &top_border, &bottom_border);

    if(center_w && !left_border && !right_border)
        cur->Camera.x = p->hitbox.x - cur->Camera.w / 2; //update camera x position if centered

    if(center_h && !bottom_border && !top_border)
        cur->Camera.y = p->hitbox.y - cur->Camera.h / 2; //update camera y position if centered
    return 0;
}

void jump(player* p, float dt) {
    p->hitbox.y -= ceil(p->vy * dt); //update player y position based on vertical velocity and delta time
    p->vy -= 7.5f; //decrease vertical velocity to simulate gravity
}

void trigger_jump(player* p) {
    p->hitbox.y = get_ground_pos(); //ensure player is on ground
    p->state = lifting; //set jump state to lifting
    p->vy = 250.0f; //set initial vertical velocity for jump
}