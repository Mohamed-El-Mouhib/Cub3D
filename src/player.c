/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 10:41:00 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 10:41:00 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * player_rotate - Rotates the player's direction vector by the player's
 * rotation angle
 *
 * this is the formula that have been:
 *     | x |    | cos(a)  -sin(a) |    | x |
 *     | y | =  | sin(a)   cos(a) | *  | y |
 * @player: Pointer to the player containing direction vector and rotation values
 * @direction: the rotation direction use ROTATE_LEFT or ROTATE_RIGHT macros
 */
void player_rotate(t_player *player, t_rotate_dir rot_dir)
{
	t_vec2 dir;
	t_vec2 plane;

	dir.x = player->dir.x * player->rot_cos - player->dir.y * player->rot_sin * rot_dir;
	dir.y = player->dir.x * player->rot_sin * rot_dir + player->dir.y * player->rot_cos;
	plane.x = player->plane.x * player->rot_cos - player->plane.y * player->rot_sin * rot_dir;
	plane.y = player->plane.x * player->rot_sin * rot_dir + player->plane.y * player->rot_cos;
	player->dir = dir;
	player->plane = plane;
}

void player_move(t_player *player, int speed, t_move_dir move_dir)
{
	if (move_dir == MOVE_LEFT)
		player->pos.x += player->dir.x * speed * -1;
	else if (move_dir == MOVE_RIGHT) 
		player->pos.x += player->dir.x * speed * 1;
	else if (move_dir == MOVE_BACKWARD)
	{
		player->pos.x += player->dir.x * speed * -1;
		player->pos.y += player->dir.y * speed * -1;
	}
	else
	{
		player->pos.x += player->dir.x * speed * 1;
		player->pos.y += player->dir.y * speed * 1;
	}
}


