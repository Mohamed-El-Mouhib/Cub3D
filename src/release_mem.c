/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release_mem.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 17:03:42 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 17:06:39 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	release_walls_paths(t_game *game)
{
	int	i;

	i = 0;
	while (i < 4)
		free(game->world.values[i++]);
}

void	release_game_and_exit(t_game *game, int status)
{
	free(game->stripes);
	release_player(game);
	release_enemies(game);
	release_assets(game);
	release_map(game);
	release_walls_paths(game);
	if (game->scene.img)
		mlx_destroy_image(game->mlx, game->scene.img);
	if (game->win)
		mlx_destroy_window(game->mlx, game->win);
	if (game->mlx)
		mlx_destroy_display(game->mlx);
	free(game->mlx);
	exit(status);
}
