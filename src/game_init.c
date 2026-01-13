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
#include <stdio.h>
#include <stdlib.h>

bool init_game_world(char *filename, t_game* game)
{
	int	i;

	i = 0;
	ft_bzero(game, sizeof(t_game));
	game->player.pos = vec2_new(-1, -1);
	if (!parse_content(filename, game))
		return (false);
	if (game->player.pos.x == -1 && game->player.pos.y == -1)
		return (false);
	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit_error("Faild to allocate mlx");
	return (true);
}

void init_game(t_game *game, char *filename)
{
	int i = 0;
	if (!init_game_world(filename, game))
		return;
	game->screen_width = 1280;
	game->screen_height = 720;
	game->assets = dyn_init_ptr();
	while (i < 2)
	{
		game->wall[i] = *assets_get_data_from_path(game, game->world.values[i]);
		i++;
	}
	game->win = mlx_new_window(game->mlx, game->screen_width, game->screen_height, "MOUSE");
	if (!game->win)
		ft_exit_error("Faild to allocate window");
	game->scene = image_new(game, game->screen_width, game->screen_height);
	game->mouse_pos = vec2_new(0, 0);
	ft_bzero(game->inputs, sizeof(game->inputs));
	player_init(game);
	game->stripes = malloc(sizeof(double) * game->screen_width);
	game->tick = curr_time_ms();
	game->shake = 0;
	int idx = assets_load_xpm(game, "./textures/digits.xpm");
	game->numbers = dyn_at(game->assets, idx);
	
	// assignment an instance to imaginary enemy
	init_enemies(game);
}

