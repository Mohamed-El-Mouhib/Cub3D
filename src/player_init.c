/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:46:16 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 16:51:10 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	player_init(t_game *game)
{
	char	dir;

	dir = get_map_cell(game, game->player.pos.x / TILE_SIZE, game->player.pos.y
			/ TILE_SIZE);
	set_cardinal_dir(dir, &game->player);
	game->player.bob = vec2_new(0, 0);
	game->player.sway = 0;
	game->player.rot_angle = INIT_ROTATION_STEP;
	game->player.max_speed = PLAYER_WALK_SPEED;
	game->player.ammo = PLAYER_MAX_AMMO;
	game->player.lives = PLAYER_MAX_LIVES;
	player_init_animations(game);
}
