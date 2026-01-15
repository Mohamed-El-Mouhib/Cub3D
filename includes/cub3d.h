/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:33:13 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 16:58:58 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "graphics.h"

# define XK_LEFT_CLICK 1

// Mouse
# define MOUSE_SENSITIVITY 0.002

// Animation Configuration
// Sway
# define PLAYER_MAX_SWAY 90
# define PLAYER_SWAY_SPEED 10

// bobbing
# define PLAYER_BOB_AMOUNT 0.09

// Moving
# define PLAYER_WALK_SPEED 60.0
# define PLAYER_RUN_SPEED 150.0
# define PLAYER_ACCEL_RATE 0.1 // How fast you reach max speed

// Live
# define PLAYER_MAX_LIVES 5

// Equipments
# define PLAYER_MAX_AMMO 15

// enemy stuff
# define ENEMY_WALK_SPEED 50.0
# define ENEMY_MAX_HEALTH 100

# define TILE_SIZE 80.0

#define MAP_VALID_CHARACTERS "01NSEWOC "

typedef struct s_lex
{
	char	*file;
	char *text;       // string text
	size_t len;       // length
	size_t pos;       // index
	size_t map_start; // index
}			t_lex;

// minimap
void		draw_minimap(t_game *game);
void		draw_fov_in_minimap(t_game *game, t_dda_ctx *dda);
void		raycast_draw_walls(t_game *game);

// Keyboard events
int			key_press(int key_code, t_game *game);
int			key_release(int key_code, t_game *game);
void		game_handle_inputs(t_game *game);

// Mouse events
int			handle_mouse_move(int x, int y, t_game *game);
int			handle_mouse_press(int keycode, int x, int y, t_game *game);
int			handle_mouse_release(int keycode, int x, int y, t_game *game);

// Utils
void		ft_exit_error(char *msg);
time_t		curr_time_ms(void);
double		sign(double x);
double		lerp(double start, double end, double t);
t_vec2		vec2_lerp(t_vec2 start, t_vec2 end, double factor);

// initializers
void		init_game(t_game *game, char *filename);
void		player_init(t_game *game);

// Parser
bool		parse_content(char *filename, t_game *game);

// release memory
void		release_map(t_game *game);
void		release_game_and_exit(t_game *game, int status);

// textures
void		init_texture_assets(t_game *game);
void		draw_texture_line(t_game *game, t_dda_ctx *info);

bool		is_valid_char(char c);

// error
void		error_indexing(void);

// assets factory
t_data		*assets_get_data_from_path(t_game *game, char *path);
int			assets_load_xpm(t_game *game, char *path);
t_animation	*init_animation(size_t start, size_t end, int duration);
t_animation	*load_animation_frames(t_game *game, char **paths, size_t size,
				int duration);
t_data		*assets_get(t_game *game, int id);

// Player
void		player_update_bobbing(t_game *game);
void		player_update_sway(t_game *game);
void		player_update_pos(t_game *game);
void		player_update_velocity(t_game *game);
void		player_update_frame(t_game *game);
void		player_update_state(t_game *game);
void		player_rotate(t_game *game, t_rotate_dir rot_dir);
void		player_move(t_player *player, t_move_dir move_dir);
void		player_render_frame(t_game *game);

// Animations
t_data		*animation_get_frame(t_game *game, t_animation *anim);
void		animation_cycle(t_game *game, t_animation *anim);

// enemy utils
void		init_enemies(t_game *game);
void		enemy_update_pos(t_game *g, t_enemy *e);
void		enemy_update_pos(t_game *g, t_enemy *e);
void		enemy_update_state(t_game *g, t_enemy *e);
void		enemy_draw_all(t_game *game);
void		enemy_update_frame_all(t_game *game);
int			can_move(t_game *g, double x, double y);

// ui
void		ui_render_aim(t_game *game);
void		ui_draw_number(t_game *game, t_vec2 pos, int number);
void		ui_render_hearts(t_game *game);
void		ui_render_ammo(t_game *game);

// access specifier
char		get_map_cell(t_game *game, int x, int y);

// error logs
bool		log_error(char *error);

void	init_side_dist_and_step(t_player *player, t_dda_ctx *dda);
void	find_first_hitting_wall(t_game *game, t_dda_ctx *dda);
void	set_left_right_side(t_dda_ctx *dda);
void	set_up_down_side(t_dda_ctx *dda);
void	init_delta_dist(t_dda_ctx *dda);

void	dda_init_map_pos(t_player *player, t_dda_ctx *dda);

t_vec2	enemy_camera_pos(t_game *game, t_enemy *enemy);
void	enemy_draw_frame(t_game *game, t_enemy *enemy);
t_vec2	enemy_get_drawing_end(t_game *game, t_enemy *enemy);
t_vec2	enemy_get_drawing_start(t_game *game, t_enemy *enemy);
unsigned int	get_point_color(t_game *game, t_enemy *enemy, int i, int j);

void	player_init_animations(t_game *game);
void	set_cardinal_dir(char dir, t_player *player);
void	player_fire_bullet(t_game *game);

#endif
