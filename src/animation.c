/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animation.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:18:21 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 16:18:22 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	animation_cycle(t_game *game, t_animation *anim)
{
	if ((size_t)(game->tick - anim->last_changed) < anim->duration)
		return ;
	anim->last_changed = game->tick;
	anim->curr++;
	if (anim->curr > anim->end)
	{
		anim->curr = anim->start;
		anim->finished = true;
	}
	else
		anim->finished = false;
}

t_data	*animation_get_frame(t_game *game, t_animation *anim)
{
	return (dyn_at(game->assets, anim->curr));
}
