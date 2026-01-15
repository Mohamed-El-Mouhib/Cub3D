/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_util_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:30:55 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 16:31:05 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * init_map_pos - set the position of player in the grip map
 *
 * @player: pointer to player
 * @dda: pointer to dda algorithm context
 */
void	dda_init_map_pos(t_player *player, t_dda_ctx *dda)
{
	dda->map_pos.x = (int)(player->pos.x / TILE_SIZE);
	dda->map_pos.y = (int)(player->pos.y / TILE_SIZE);
}
