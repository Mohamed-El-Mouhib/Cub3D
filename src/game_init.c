/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:05:39 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 11:05:39 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int map[MAP_HEIGHT][MAP_WIDTH] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
		{1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 1, 1, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
		};


// static t_world init_game_world(char *filename)
// {
// 	t_world world;
//
//
//
//
// }

void init_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit_error("Faild to allocate mlx");
	game->world.map_width = MAP_WIDTH;
	game->world.map_height = MAP_HEIGHT;
	game->world.map = map;
	game->screen_width = MAP_WIDTH * TILE_SIZE;
	game->screen_height = MAP_HEIGHT * TILE_SIZE;
	game->win = mlx_new_window(game->mlx, game->screen_width, game->screen_height, "Hello world!");
	if (!game->win)
		ft_exit_error("Faild to allocate window");
	game->scene = image_new(game, game->screen_width, game->screen_height);
	game->mouse_pos = vec2_new(0, 0);
	game->player.pos = vec2_new(game->screen_width / 2.0, game->screen_height / 2.0);
	// player looking up
	game->player.dir = vec2_new(-1, 0);
	// field of view is 60 degree
	game->player.plane = vec2_new(0, 0.66);

	// The rotation angle calcualte the cos,sin once to cache it to reuse later
	game->player.rot_angle = INIT_ROTATION_STEP;
	game->player.rot_cos = cos(INIT_ROTATION_STEP);
	game->player.rot_sin = sin(INIT_ROTATION_STEP);
	ft_bzero(game->keyboard_events, sizeof(game->keyboard_events));
}

