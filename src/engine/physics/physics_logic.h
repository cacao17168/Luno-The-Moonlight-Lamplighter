#ifndef PHYSICS_LOGIC_H_
#define PHYSICS_LOGIC_H_

#define JUMP_SCALE 100 //maximum jump height
#define ACCELERATION 500 //player acceleration

int get_camera_center_w(game_properties *cur);

int get_camera_center_h(game_properties *cur);

void get_camera_borders(game_properties *cur, int* left, int* right, int* top, int* bottom);

int is_centered_x(player* p, game_properties *cur);

int is_centered_y(player* p, game_properties *cur);

int is_player_on_ground(player* p, game_properties *cur);

int calc_horizontal_mov(player* p, float dt);

int update_player_speed(player* p, keytype keys[], float dt);

int update_camera_pos(game_properties *cur, player* p);

#endif