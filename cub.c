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
#include <mlx.h>
#include <stdio.h>


// RGB colors

#define FRAME_RATE 100
# define MAP_HEIGHT 10
# define MAP_WIDTH 10
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
	game->map_width = MAP_WIDTH;
	game->map_height = MAP_HEIGHT;
	game->screen_width = MAP_WIDTH * TILE_SIZE;
	game->screen_height = MAP_HEIGHT * TILE_SIZE;
	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit_error("Faild to allocate mlx");
	game->win = mlx_new_window(game->mlx, game->screen_width, game->screen_height, "Hello world!");
	if (!game->win)
		ft_exit_error("Faild to allocate window");
	game->mouse_pos = vec2_new(0, 0);
	game->scene = image_new(game, game->screen_width, game->screen_height);
	game->player = vec2_new(game->screen_width / 2.0, game->screen_height / 2.0);
}

int	key_even_handler(int key_code, t_game *game)
{
	if (key_code == XK_Escape)
	{
		exit(0);
	}
	if (key_code == 'd')
	{
		game->player.x++;
	}
	if (key_code == 'a')
	{
		game->player.x--;
	}
	if (key_code == 'w')
	{
		game->player.y++;
	}
	if (key_code == 's')
	{
		game->player.y--;
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

time_t	curr_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
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

void vec2_print(t_vec2 v, char *prefix)
{
	printf("%s: P(%f, %f)\n", prefix, v.x, v.y);
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
	p2.x -= (0.1 * (sign(sub.x)));
	p2.y -= (0.1 * (sign(sub.y)));
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

int game_loop(t_game *game)
{
	static time_t last_frame_time;
	static size_t curr_time;

	if (curr_time == 10)
	{
		size_t avg = average(times, curr_time);
		if (avg > 0)
			printf("AVG_FRAME_RATE: %ld\n", 1000/ avg);
		curr_time = 0;
	}
	times[curr_time++] = curr_time_ms() - last_frame_time;
	last_frame_time = curr_time_ms();
	image_clear(&game->scene);
	// draw_grid(&game->scene);
	draw_filled_square(&game->scene, vec2_new(0, 0), TILE_SIZE, PURPLE);
	draw_circle(&game->scene, game->mouse_pos, 5, RED);
	draw_line(&game->scene, game->player, game->mouse_pos, RED);
	draw_circle(&game->scene, game->player, 5, RED);
	intersection_points(&game->scene, game->player, game->mouse_pos);
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
