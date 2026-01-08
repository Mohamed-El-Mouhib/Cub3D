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
void player_rotate(t_game *game, t_rotate_dir rot_dir)
{
	t_player *player;
	t_vec2 plane;
	t_vec2 dir;
	double c;
	double s;

	player = &game->player;
	c = cos(INIT_ROTATION_STEP * game->dt);
	s = sin(INIT_ROTATION_STEP * game->dt);
	dir.x = player->dir.x * c - player->dir.y * s * rot_dir;
	dir.y = player->dir.x * s * rot_dir + player->dir.y * c;
	plane.x = player->plane.x * c - player->plane.y * s * rot_dir;
	plane.y = player->plane.x * s * rot_dir + player->plane.y * c;
	player->dir = dir;
	player->plane = plane;
}
