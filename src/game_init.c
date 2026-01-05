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

bool init_game_world(char *filename, t_game* game)
{
	ft_bzero(game, sizeof(t_game));
	game->player.pos = vec2_new(-1, -1);
	if (!parse_content(filename, game))
		return (false);
	if (game->player.pos.x == -1 && game->player.pos.y == -1)
		return (init_error(NO_PLAYER, 0,0, NULL), false);
	return (true);
}

void init_game(t_game *game, char *filename)
{
	if (!init_game_world(filename, game))
	{
		error_indexing();
		return;
	}
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
	game->stripes = malloc(sizeof(double) * game->screen_width);
	
	// assignment an instance to imaginary enemy
	init_enemies(game);
}

