/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   release_mem_utis_1.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 17:04:16 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 17:08:01 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	release_player(t_game *game)
{
	int	i;

	i = 0;
	while (i < PLAYER_STATS_NBR)
		free(game->player.animations[i++]);
}

void	release_enemy(void *e)
{
	int		i;
	t_enemy	*enemy;

	enemy = e;
	if (!enemy)
		return ;
	i = 0;
	while (i < ENEMY_STATS_NBR)
		free(enemy->animation[i++]);
	free(enemy);
}

void	release_enemies(t_game *game)
{
	if (!game->enemies)
		return ;
	dyn_erase(game->enemies, release_enemy);
	free(game->enemies);
}

void	release_assets(t_game *game)
{
	size_t	i;
	t_data	*data;

	if (!game->assets)
		return ;
	i = 0;
	while (i < game->assets->length)
	{
		data = game->assets->buff[i];
		if (data && data->img)
			mlx_destroy_image(game->mlx, data->img);
		i++;
	}
	dyn_erase(game->assets, free);
	free(game->assets);
}

void	release_map(t_game *game)
{
	int	i;

	if (!game->world.map)
		return ;
	i = 0;
	while (i < game->world.map_height)
	{
		free(game->world.map[i]);
		i++;
	}
	free(game->world.map);
	game->world.map = NULL;
}
