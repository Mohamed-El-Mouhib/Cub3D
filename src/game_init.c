/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:05:39 by aljbari           #+#    #+#             */
/*   Updated: 2025/11/17 22:49:38 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	evaluate_file_content(t_dyn *map, t_world *world)
{
}

t_world init_game_world(char *filename)
{
	t_world world;
	t_dyn map;

	ft_bzero(&world, sizeof(world));
	map = read_map_from_file(filename);
	world.map = (char **)map.buff;
	world.map_width = ft_strlen(world.map[0]);
	world.map_height = map.length;
	return (world);
}

void init_game(t_game *game)
{
	game->world = init_game_world("map.txt");
	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit_error("Faild to allocate mlx");
	// mlx_get_screen_size(game->mlx, (int *)&game->screen_width, (int *)&game->screen_height);
	game->screen_width = 1280;
	game->screen_height = 720;
	game->win = mlx_new_window(game->mlx, game->screen_width, game->screen_height, "MOUSE");
	if (!game->win)
		ft_exit_error("Faild to allocate window");
	game->scene = image_new(game, game->screen_width, game->screen_height);
	game->mouse_pos = vec2_new(0, 0);
	game->player.pos = vec2_new(TILE_SIZE * 4, TILE_SIZE * 4);
	// player looking up
	game->player.dir = vec2_new(1, 0);
	// field of view is 60 degree
	game->player.plane = vec2_new(0, 0.66);

	// The rotation angle calcualte the cos,sin once to cache it to reuse later
	game->player.rot_angle = INIT_ROTATION_STEP;
	game->player.rot_cos = cos(INIT_ROTATION_STEP);
	game->player.rot_sin = sin(INIT_ROTATION_STEP);
	ft_bzero(game->keyboard_events, sizeof(game->keyboard_events));
	
	// an assign an instance to imaginary enemy so i can implement one
	game->enemy.pos = vec2_new(TILE_SIZE * 2, TILE_SIZE * 2);
}

