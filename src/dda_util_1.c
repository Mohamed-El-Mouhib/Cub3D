/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda_util_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:31:26 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 16:31:27 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/**
 * init_delta_dist - initialize ray delta distances
 * @dda: pointer to dda algorithm context
 *
 * Delta distances represent the length a ray has to travel in order to
 * move exactly one tile unit in the x or y direction
 */
void	init_delta_dist(t_dda_ctx *dda)
{
	if (dda->ray.x != 0)
		dda->delta_dist.x = (fabs(1.0 / dda->ray.x) * TILE_SIZE);
	else
		dda->delta_dist.x = 1e30;
	if (dda->ray.y != 0)
		dda->delta_dist.y = (fabs(1.0 / dda->ray.y) * TILE_SIZE);
	else
		dda->delta_dist.y = 1e30;
}

/**
 * find_first_hitting_wall - run DDA loop until a wall or map edge is hit
 * This will aslo set the side of wall
 *
 * @game: pointer to game context
 * @dda: pointer to dda algorithm context
 *
 */
void	set_up_down_side(t_dda_ctx *dda)
{
	if (dda->step_dir.y > 0)
		dda->side = WALL_NORTH;
	else
		dda->side = WALL_SOUTH;
}

void	set_left_right_side(t_dda_ctx *dda)
{
	if (dda->step_dir.x > 0)
		dda->side = WALL_WEST;
	else
		dda->side = WALL_EAST;
}

void	find_first_hitting_wall(t_game *game, t_dda_ctx *dda)
{
	while (1)
	{
		if (dda->side_dist.x < dda->side_dist.y)
		{
			dda->side_dist.x += (dda->delta_dist.x * TILE_SIZE);
			dda->map_pos.x += (int)dda->step_dir.x;
			set_left_right_side(dda);
		}
		else
		{
			dda->side_dist.y += (dda->delta_dist.y * TILE_SIZE);
			dda->map_pos.y += (int)dda->step_dir.y;
			set_up_down_side(dda);
		}
		if (dda->map_pos.x < 0
			|| dda->map_pos.x >= game->world.map_width
			|| dda->map_pos.y < 0
			|| dda->map_pos.y >= game->world.map_height)
			break ;
		if ((game->world.map)[(int)dda->map_pos.y]
		[(int)dda->map_pos.x] == '1' || (game->world.map)
		[(int)dda->map_pos.y][(int)dda->map_pos.x] == 'C')
			break ;
	}
}

/**
 * init_side_dist_and_step - compute step direction and initial side distance
 * - side distances (distance to first x/y wall grid)
 * - step direction (which way the we will move in the map)
 *
 * @player: pointer to player
 * @dda: pointer to dda algorithm context
 */
void	init_side_dist_and_step(t_player *player, t_dda_ctx *dda)
{
	if (dda->ray.x < 0)
	{
		dda->side_dist.x = dda->delta_dist.x * (player->pos.x - dda->map_pos.x
				* TILE_SIZE);
		dda->step_dir.x = -1.0;
	}
	else
	{
		dda->side_dist.x = dda->delta_dist.x * ((dda->map_pos.x + 1) * TILE_SIZE
				- player->pos.x);
		dda->step_dir.x = 1.0;
	}
	if (dda->ray.y < 0)
	{
		dda->side_dist.y = dda->delta_dist.y * (player->pos.y - dda->map_pos.y
				* TILE_SIZE);
		dda->step_dir.y = -1.0;
	}
	else
	{
		dda->side_dist.y = dda->delta_dist.y * ((dda->map_pos.y + 1) * TILE_SIZE
				- player->pos.y);
		dda->step_dir.y = 1.0;
	}
}
