/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari    <aljbari@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 18:53:45 by your_login        #+#    #+#             */
/*   Updated: 2025/11/17 22:39:41 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include "dynamic_array.h"
# include "libft.h"
# include <X11/keysym.h>
# include <fcntl.h>
# include <limits.h>
# include <math.h>
# include <mlx.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define INIT_ROTATION_STEP 3.0

# define MINIMAP_SCALE 4.0
# define MINIMAP_LEN 200
# define HALF_MINIMAP 100
# define SQR_REDIOUS 10000
# define MINIMAP_ZOOM 4
# define X_OFFSET 10
# define Y_OFFSET 10

typedef int			t_color;

/**
 * t_vec2 - Vectore represeting point at the screen
 *
 * @x: x-coordinate
 * @y: y-coordinate
 */
typedef struct s_vec2
{
	double			x;
	double			y;
}					t_vec2;

typedef	struct e_ivec
{
	int	x;
	int	y;
}	t_ivec;

typedef struct s_color
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
}					t_color_;

typedef struct s_data
{
	void			*img;
	char			*addr;
	int				bpp;
	int				line_len;
	int				endian;
	int				width;
	int				height;
}					t_data;

typedef struct s_animation
{
	size_t			start;
	size_t			end;
	size_t			curr;
	size_t			duration;
	bool			finished;
	time_t			last_changed;
}					t_animation;

enum				e_player_stats
{
	PLAYER_SHOOTING = 0,
	PLAYER_RELOAD,
	PLAYER_WALKING,
	PLAYER_STATS_NBR
};

typedef enum e_enemy_stats
{
	ENEMY_WALKING = 0,
	ENEMY_ATTACKING,
	ENEMY_DEAD,
	ENEMY_STATS_NBR
}					t_enemy_stats;

typedef enum e_tok
{
	TOKEN_NO = 0,
	TOKEN_SO,
	TOKEN_WE,
	TOKEN_EA,
	TOKEN_F,
	TOKEN_C,
	TOKEN_NBR,
	TOKEN_INVALID
}					t_tok;

typedef enum e_wall_side
{
	WALL_NORTH,
	WALL_SOUTH,
	WALL_EAST,
	WALL_WEST,
	WALL_DOOR,
	WALL_NBR
}					t_wall_side;

typedef struct s_player
{
	t_vec2			dir;
	t_vec2			plane;
	t_vec2			pos;
	t_vec2			bob;
	double			sway;
	t_vec2			velocity;
	t_vec2			input_dir;
	double			speed;
	double			max_speed;
	t_animation		*animations[PLAYER_STATS_NBR];
	size_t			state;
	double			rot_angle;
	int				lives;
	int				ammo;
}					t_player;

typedef struct s_world
{
	char			*values[4];
	int				map_width;
	int				map_height;
	char			**map;
}					t_world;

/*
 * @t_framse:	struct holding wall textures and their boundaries
 * @image:	array of t_data, each storing image info (bpp, address,
		line length, endian)
 */

typedef struct e_config
{
	char			*value[TOKEN_NBR];
	char			**map;
	size_t			map_width;
	size_t			map_height;
	int				player_x;
	int				player_y;
	char			player_dir;
	t_color			f;
	t_color			c;
}					t_config;

/**
 * @s: draw start
 * @e: draw end
 * @pos: player position
 * @size: size of the enemy rendered box
 * @camera: position of enemy relative to the player camera
 * @screen: intersaction with the screen projection
 */
typedef struct s_enemy
{
	t_vec2			s;
	t_vec2			e;
	t_vec2			pos;
	int				size;
	t_enemy_stats	state;
	t_vec2			camera;
	t_animation		*animation[ENEMY_STATS_NBR];
	time_t			last_attack_time;
	double			screen;
	int				health;
}					t_enemy;

typedef enum e_ui_img
{
	UI_DIGITS = 0,
	UI_HEARTH,
	UI_BULLET_CONTAINER,
	UI_OPEN_DOOR,
	UI_CLOSE_DOOR,
	UI_AIM,
	UI_NBR,
}					t_ui_img;

typedef struct s_game
{
	t_world			world;
	t_player		player;
	t_vec2			mouse_pos;
	t_vec2			last_mouse_pos;
	size_t			screen_width;
	size_t			screen_height;
	bool			inputs[256];
	t_data			scene;
	void			*win;
	void			*mlx;
	t_color			ceiling;
	t_color			floor;
	t_data			wall[WALL_NBR];
	double			*stripes;
	t_dyn			*enemies;
	t_dyn			*assets;
	time_t			tick;
	double			shake;
	double			dt;
	t_data			*ui[UI_NBR];
}					t_game;

/**
 *       ^
 *       N
 *  < E     W >
 *       S
 *       🢓
 */
typedef enum e_keycode
{
	KEY_ESCAPE = 0,
	KEY_SHIFT_L,
	KEY_LCLICK,
	KEY_SPACE,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_DOWN,
	KEY_UP,
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_R,
}					t_keycode;

/**
 * t_dda_algo - the dda context
 *
 * @delta_dist:    Distance between next x-side and next y-side
 * @side_dist:     Distance from player to first x-side / y-side
 * @step_dir:      Where to move next in the grid in x-side and y-side
 * @ray_size:      Normalized ray size relative to screen
 * @hit_dist:      Perpendicular distance to first wall hit
 * @map_pos:       Current cell position in map grid
 * @ray:           Current ray direction
 * @side:          this hit side (NORTH, ...)
 * @wall_top:      Screen coordinates of top of wall slice
 * @wall_bottom:   Screen coordinates of bottom of wall slice
 */
typedef struct s_dda_alog
{
	t_vec2			delta_dist;
	t_vec2			side_dist;
	t_vec2			step_dir;
	t_vec2			line_start;
	t_vec2			line_end;
	t_vec2			map_pos;
	t_vec2			ray;
	double			ray_size;
	double			hit_dist;
	double			line_height;
	t_wall_side		side;
}					t_dda_ctx;

// player movements
//
typedef enum e_rotate_dir
{
	ROTATE_LEFT = -1,
	ROTATE_RIGHT = 1
}					t_rotate_dir;

typedef enum e_move_dir
{
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
}					t_move_dir;

#endif
