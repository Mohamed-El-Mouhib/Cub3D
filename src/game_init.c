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

// void	evaluate_file_content(t_dyn *map, t_world *world)
// {
// }

t_world init_game_world(char *filename)
{
	t_world world;
	t_dyn map;

	ft_bzero(&world, sizeof(world));
	map = read_map_from_file(filename);
	world.map = (char **)map.buff;
	world.map_width = ft_strlen(world.map[0]); //must be changed
	world.map_height = map.length; //must be changed
	return (world);
}

void init_game(t_game *game)
{
	ft_bzero(game, sizeof(*game));
	game->world = init_game_world("map.cub");
	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit_error("Faild to allocate mlx");
	game->screen_width = 1280;
	game->screen_height = 720;
	game->win = mlx_new_window(game->mlx, game->screen_width, game->screen_height, "MOUSE");
	if (!game->win)
		ft_exit_error("Faild to allocate window");
	game->scene = image_new(game, game->screen_width, game->screen_height);
	game->mouse_pos = vec2_new(0, 0);
	game->assets = dyn_init_ptr();
	ft_bzero(game->keyboard_events, sizeof(game->keyboard_events));
	init_player(game);
	
	// assignment an instance to imaginary enemy
	game->enemy.pos = vec2_new(TILE_SIZE * 2, TILE_SIZE * 2);
}

