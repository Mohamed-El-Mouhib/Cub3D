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

static char get_map_cell(t_game *game, int x, int y)
{
	if (x >= (int)game->world.map_width || y >= (int)game->world.map_height || x < 0 || y < 0)
		return ('1');
	return (game->world.map[y][x]);
}


bool enemy_foreach(t_game *game, t_vec2 bullet)
{
	t_enemy *enemy;
	double d;
	double max_d;

	d = (15*15);
	max_d = TILE_SIZE * TILE_SIZE * 8;

	for (size_t i = 0; i < game->enemies->length; i++)
	{
		enemy = dyn_at(game->enemies, i);
		if (enemy->health > 0 && vec2_len_squared(bullet, enemy->pos) < d)
		{
			enemy->health -= 7;
			printf("ENEMY HEALTH: %d\n", enemy->health);
			if (enemy->health <= 0)
			{
				enemy->health = 0;
				printf("You killed an enemy\n");
			}
			return (true);
		}
	}
	return (false);
}


void player_fire_bullet(t_game *game)
{
	t_vec2 bullet;

	bullet = game->player.pos;
	while (1)
	{
		bullet = vec2_add(bullet, game->player.dir);
		if (get_map_cell(game, (int)bullet.x / TILE_SIZE, (int)bullet.y / TILE_SIZE) == '1')
			return ;
		if (enemy_foreach(game, bullet))
			return ;
	}
}
