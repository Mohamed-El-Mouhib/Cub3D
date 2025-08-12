/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:54:38 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/29 19:51:51 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include "includes/graphics.h"
#include "includes/types.h"


time_t	curr_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

# define FRAME_RATE 100
# define MAP_HEIGHT 10
# define MAP_WIDTH 10
# define INIT_ROTATION_STEP_DEGREE 5
# define INIT_ROTATION_STEP ((INIT_ROTATION_STEP_DEGREE * M_PI) / 180.0)

int map[MAP_HEIGHT][MAP_WIDTH] = {
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

t_vec2 intersect;


void ft_exit_error(char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));	
	exit(1);
}

void init_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit_error("Faild to allocate mlx");

	game->world.map_width = MAP_WIDTH;
	game->world.map_height = MAP_HEIGHT;
	game->screen_width = MAP_WIDTH * TILE_SIZE;
	game->screen_height = MAP_HEIGHT * TILE_SIZE;
	game->win = mlx_new_window(game->mlx, game->screen_width, game->screen_height, "Hello world!");
	if (!game->win)
		ft_exit_error("Faild to allocate window");

	game->scene = image_new(game, game->screen_width, game->screen_height);
	game->mouse_pos = vec2_new(0, 0);
	game->player.pos = vec2_new(game->screen_width / 2.0, game->screen_height / 2.0);
	// player looking up
	game->player.dir = vec2_new(0, -1);
	// field of view is 60 degree 
	game->player.plane = vec2_new(0, 0.66);

	// The rotation angle calcualte the cos,sin once to cache it for reuse later
	game->player.rot_angle = INIT_ROTATION_STEP;
	game->player.rot_cos = cos(INIT_ROTATION_STEP);
	game->player.rot_sin = sin(INIT_ROTATION_STEP);
}

typedef enum e_rotate_dir
{
	ROTATE_LEFT  = -1,
	ROTATE_RIGHT = 1
} t_rotate_dir;

typedef enum e_move_dir
{
	FORWARD  = 1,
	BACKWARD = -1
} t_move_dir;

/**
 * player_rotate - Rotates the player's direction vector by the player's
 * rotation angle
 *
 * this is the formula that have been:
 *     | x |    | cos(a)  -sin(a) |    | x |
 *     | y | =  | sin(a)   cos(a) | *  | y |
 * @player: Pointer to the player containing direction vector and rotation values
 * @direction: the rotation direction use ROTATE_LEFT or ROTATE_RIGHT macros
 */
void player_player(t_player *player, t_rotate_dir rot_dir)
{
	t_vec2 dir;

	dir.x = player->dir.x * player->rot_cos - player->dir.y * player->rot_sin * rot_dir;
	dir.y = player->dir.x * player->rot_sin * rot_dir + player->dir.y * player->rot_cos;
	player->dir = dir;
}

void player_move_forward(t_player *player, int speed, t_move_dir move_dir)
{
	player->pos.x += player->dir.x * speed * move_dir;
	player->pos.y += player->dir.y * speed * move_dir;
}


time_t time = 0;

int	key_even_handler(int key_code, t_game *game)
{
	t_player *player;

	player = &game->player;
	if (key_code == XK_Escape)
	{
		exit(0);
	}
	if (key_code == 'd')
	{
		rotate_player(player, ROTATE_RIGHT);
	}
	if (key_code == 'a')
	{
		rotate_player(player, ROTATE_LEFT);
	}
	if (key_code == 'w')
	{
		player_move_forward(player, 10);
	}
	if (key_code == 's')
	{
		player_move_backward(player, 10);
	}
	return (0);
}

void draw_grid(t_data  *buff)
{
	t_vec2 start;
	int i;

	start = vec2_new(0, 0);
	i = 0;
	while (i <= buff->width)
	{
		start.y = i;
		draw_horizontal_line(buff, start, buff->width, RED);
		i+= TILE_SIZE;
	}
	start = vec2_new(0, 0);
	i = 0;
	while (i <= buff->width)
	{
		start.x = i;
		draw_vertical_line(buff, start, buff->height, RED);
		i+= TILE_SIZE;
	}
}

int handle_mouse_event(int x,int y, t_game *game)
{
	game->mouse_pos.x = x;
	game->mouse_pos.y = y;
	return (0);
}



double (*rounding_func(double dx))(double)
{
	if (dx >= 0)
		return (floor);
	else
		return (ceil);
}

double sign(double x)
{
	return ((x > 0) - (x < 0));
}

bool out_of_boundries(t_data *scene, t_vec2 a)
{
	return (a.x < 0 || a.y < 0 || a.x > scene->width || a.y > scene->height);
}

void	intersection_points(t_data *scene, t_vec2 p1, t_vec2 p2)
{
	double k;
	double c;
	t_vec2 sub;
	double x,y;
	t_vec2 intersect;
	t_vec2 next_x, next_y;

	sub = vec2_sub(p1, p2);
	if (sub.x == 0)
		k = 0;
	else
		k = sub.y / sub.x;
	c = p1.y - k*p1.x;
	// p2.x -= (0.1 * (sign(sub.x)));
	// p2.y -= (0.1 * (sign(sub.y)));
	x = rounding_func(sub.x)(p2.x / TILE_SIZE)*TILE_SIZE;
	y = k*x + c;
	next_x = vec2_new(x, y);
	y = rounding_func(sub.y)(p2.y / TILE_SIZE)*TILE_SIZE;
	if (k != 0)
		x = (y - c) / k;
	else
		x = c;
	next_y = vec2_new(x, y);
	double len_x = vec2_len_squared(next_x, p2);
	double len_y = vec2_len_squared(next_y, p2);
	intersect = len_x > len_y ? next_y : next_x;
	draw_circle(scene, intersect, 6, PURPLE);
	return ;
}


int times[10];

/**
 * average - calculate the average of numbers
 *
 * @stats: array of numbers
 * @size: array size
 *
 * Return: average value
 */
size_t average(int *stats, size_t size)
{
	size_t sum;
	size_t i;

	i = 0;
	sum = 0;
	while (i < size)
	{
		sum += stats[i++];
	}
	return (sum/size);
}

// time stats to be added later:
// static time_t last_frame_time;
// static size_t curr_time;
//
// if (curr_time == 10)
// {
// 	size_t avg = average(times, curr_time);
// 	if (avg > 0)
// 		printf("AVG_FRAME_RATE: %ld\n", 1000/ avg);
// 	curr_time = 0;
// }
// times[curr_time++] = curr_time_ms() - last_frame_time;
// last_frame_time = curr_time_ms();
void draw_player(t_game *game)
{
	double scale = 40.0;
	t_vec2 arrow_end;

	arrow_end = vec2_add(vec2_scale(game->player.dir, scale), game->player.pos);
	draw_line(&game->scene, game->player.pos, arrow_end, PURPLE);
	draw_filled_circle(&game->scene, game->player.pos, 6, RED);
	draw_filled_square(&game->scene, arrow_end, 4, PURPLE);
}

int game_loop(t_game *game)
{
	image_clear(&game->scene);
	draw_grid(&game->scene);
	// draw_filled_square(&game->scene, vec2_new(0, 0), TILE_SIZE, PURPLE);
	// draw_circle(&game->scene, game->mouse_pos, 5, RED);
	// draw_line(&game->scene, game->player.pos, game->mouse_pos, RED);
	// draw_circle(&game->scene, game->player.pos, 5, RED);
	// intersection_points(&game->scene, game->player.pos, game->mouse_pos);
	draw_player(game);
	mlx_put_image_to_window(game->mlx, game->win,game->scene.img, 0, 0);
	return (0);
}

int	main()
{
	t_game game;


	init_game(&game);

	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_hook(game.win, 02, 1L<<0, key_even_handler, &game);
	mlx_hook(game.win, 03, 1L<<1, key_even_handler, &game);
	mlx_hook(game.win, 06, 1L<<6, handle_mouse_event, &game); // for mouse
	mlx_loop(game.mlx);
	return (0);
}
