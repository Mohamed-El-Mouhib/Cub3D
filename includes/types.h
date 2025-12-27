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

# include "libft.h"
# ifdef __APPLE__
# include "../../mlx/mlx.h"
#  define XK_Escape 53
#  define XK_Right 123
#  define XK_Left 124
#  define XK_a 0
#  define XK_s 1
#  define XK_d 2
#  define XK_w 13
#  define XK_r 15
# else
#  include <X11/keysym.h>
# include <mlx.h>
# endif
# include <fcntl.h>
# include <math.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define FRAME_RATE 24
# define INIT_ROTATION_STEP_DEGREE 2
# define INIT_ROTATION_STEP ((INIT_ROTATION_STEP_DEGREE * M_PI) / 180.0)

typedef int t_color;

/**
 * t_vec2 - Vectore represeting point at the screen
 *
 * @x: x-coordinate
 * @y: y-coordinate
 */
typedef struct s_vec2
{
	double	x;
	double	y;
}			t_vec2;

typedef enum e_error
{
	NO_FILENAME,
	EMPY_FILE,
	INVALID_FILENAME,
	INVALID_CHAR,
	NO_ERR,
}	t_error;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}			t_data;

typedef struct s_player
{
	t_vec2 dir;
	t_vec2 plane;
	t_vec2 pos;
	double rot_angle; // the rotation step angle
	double rot_cos;   // cos of rotation step, calculate it once
	double rot_sin;   // sin of rotation step, calculate it once
}			t_player;

typedef struct s_world {
	size_t  map_width;
	size_t  map_height;
	char **map;
	t_color	ceiling;
	t_color	floor;
}
t_world;

/*
 * @t_framse:	struct holding wall textures and their boundaries
 * @image:	array of t_data, each storing image info (bpp, address, line length, endian)
 */
typedef struct s_frames
{
	t_data	walltex_[2];
}	t_frames;

typedef enum
{
	CHASE,
	IDLE,
}	t_stat;

/*
 * @instance:	the instance of the enemy
 * @pos:			vectore holding enemy position in map
 * @t_ai:		struct holding the instance of the enemy AI if they exist in the map
 * @t_stat:		enum holding the status of the enemy
 * @speed:		movement speed of the enemy
 */
typedef struct s_char
{
	t_data	instance;
	t_vec2	pos;
	float		speed;
	t_stat	stat;
}	t_ai;

typedef struct s_game
{
	t_world  world;
	t_player player;
	t_vec2	 mouse_pos;
	// Rander fields
	size_t  screen_width;
	size_t  screen_height;
	bool keyboard_events[256];
	t_data	 scene;
	t_frames	frames;
	void	 *win;
	void	 *mlx;
	t_ai	enemy;
}			t_game;

/**
 *       ^
 *       N
 *  < E     W >
 *       S
 *       🢓
 */
typedef enum e_wall_side
{
	WALL_NORTH, 
	WALL_SOUTH,
	WALL_EAST,
	WALL_WEST
}	t_wall_side;

typedef enum e_keycode{
	KEY_ESCAPE = 0,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_DOWN,
	KEY_UP,
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_R,
} t_keycode;

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
	t_vec2 delta_dist;
	t_vec2 side_dist;
	t_vec2 step_dir;
	t_vec2 line_start;
	t_vec2 line_end;
	t_vec2 map_pos;
	t_vec2 ray;
	double ray_size;
	double hit_dist;
	double line_height;
	t_wall_side side;
} t_dda_ctx;

// player movements
//
typedef enum e_rotate_dir
{
	ROTATE_LEFT  = -1,
	ROTATE_RIGHT = 1
} t_rotate_dir;

typedef enum e_move_dir
{
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
} t_move_dir;

#endif
