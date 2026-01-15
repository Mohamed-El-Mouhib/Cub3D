/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_movements_1.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:07:30 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:14:18 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int		has_los(t_game *g, t_vec2 from, t_vec2 to);
bool	enemy_is_too_close(t_game *game, t_enemy *enemy);

void	enemy_update_pos(t_game *game, t_enemy *enemy)
{
	t_vec2	dir;
	t_vec2	next;

	if (enemy->state == ENEMY_DEAD)
		return ;
	if (enemy_is_too_close(game, enemy))
		return ;
	if (!has_los(game, enemy->pos, game->player.pos))
		return ;
	dir = vec2_unit(enemy->pos, game->player.pos);
	dir = vec2_scale(dir, ENEMY_WALK_SPEED * game->dt);
	next.x = enemy->pos.x + dir.x;
	if (can_move(game, next.x, enemy->pos.y))
		enemy->pos.x = next.x;
	next.y = enemy->pos.y + dir.y;
	if (can_move(game, enemy->pos.x, next.y))
		enemy->pos.y = next.y;
}
