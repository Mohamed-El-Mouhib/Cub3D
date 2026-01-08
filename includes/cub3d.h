/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:33:13 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/09 19:33:13 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CUB3D_H
# define CUB3D_H
# include "graphics.h"

// Animation Configuration
// Sway
#define PLAYER_MAX_SWAY 90
#define PLAYER_SWAY_SPEED 10

// bobbing
#define PLAYER_BOB_AMOUNT 0.15

// Moving
#define PLAYER_WALK_SPEED 40.0
#define PLAYER_RUN_SPEED 80.0
#define PLAYER_ACCEL_RATE 0.1   // How fast you reach max speed

// Live
#define PLAYER_MAX_LIVES 5

// Equipments
#define PLAYER_MAX_AMMO 15

// enemy stuff
#define ENEMY_WALK_SPEED 40


# define TILE_SIZE 80.0
// # define WIN_H (TILE_SIZE * 20 + 1)
// # define WIN_W (TILE_SIZE * 40 + 1)

// minimap
void draw_minimap(t_game *game);
void draw_fov_in_minimap(t_game *game, t_dda_ctx *dda);
void raycast_draw_walls(t_game *game);

// Keyboard events
int	key_press(int key_code, t_game *game);
int	key_release(int key_code, t_game *game);
void    game_handle_keyboard_events(t_game *game);

// Mouse events
int handle_mouse_event(int x,int y, t_game *game);

// Utils
void ft_exit_error(char *msg);
time_t	curr_time_ms(void);
double sign(double x);
double lerp(double start, double end, double t);
t_vec2 vec2_lerp(t_vec2 start, t_vec2 end, double factor);

// initializers
void init_game(t_game *game, char *filename);
void player_init(t_game *game);

// Parser
bool parse_content(char *filename, t_game* game);
void	init_error(t_error_type type, int line, int index, void* p);

// textures
void	draw_texture_line(t_game *game, t_dda_ctx *info);

bool is_valid_char(char c);

// error
void	error_indexing(void);
// assets factory
int assets_load_xpm(t_game *game, char *path);
t_animation *init_animation(size_t start, size_t end, int duration);
t_animation *load_animation_frames(t_game *game, char **paths, size_t size);
t_data *assets_get(t_game *game, int id);

// Player
void player_update_bobbing(t_game *game);
void player_update_sway(t_game *game);
void player_update_pos(t_game *game);
void player_update_velocity(t_game *game);
void player_update_frame(t_game *game);
void player_update_state(t_game *game);
void player_rotate(t_game *game, t_rotate_dir rot_dir);
void player_move(t_player *player, t_move_dir move_dir);
void player_render_frame(t_game *game);


// Animations
t_data *animation_get_frame(t_game *game, t_animation *anim);
void animation_cycle(t_game *game, t_animation *anim);

// enemy utils
void init_enemies(t_game *game);
void enemy_update_pos(t_game *g, t_enemy *e);
void enemy_update_pos(t_game *g, t_enemy *e);
void enemy_update_state(t_game *g, t_enemy *e);
void enemy_draw_all(t_game *game);
void enemy_update_frame_all(t_game *game);

#endif
