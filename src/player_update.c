/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 17:01:45 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 17:02:18 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * player_update_frame - update the current player frame
 */
void	player_update_frame(t_game *game)
{
	animation_cycle(game, game->player.animations[game->player.state]);
}
