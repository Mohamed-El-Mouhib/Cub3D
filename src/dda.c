/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 10:26:43 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 10:26:43 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * init_delta_dist - initialize ray delta distances
 * @dda: pointer to dda algorithm context
 *
 * Delta distances represent the length a ray has to travel in order to
 * move exactly one tile unit in the x or y direction
 */
void init_delta_dist(t_dda_ctx *dda)
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
void find_first_hitting_wall(t_game *game, t_dda_ctx *dda)
{
	while (1)
	{
		if (dda->side_dist.x < dda->side_dist.y)
		{
			dda->side_dist.x += (dda->delta_dist.x * TILE_SIZE);
			dda->map_pos.x += (int)dda->step_dir.x;
			dda->side = 0;
		}
		else
		{
			dda->side_dist.y += (dda->delta_dist.y * TILE_SIZE);
			dda->map_pos.y += (int)dda->step_dir.y;
			dda->side = 1;
		}
		if (dda->map_pos.x < 0 || dda->map_pos.x >= game->world.map_width || dda->map_pos.y < 0 || dda->map_pos.y >= game->world.map_height)
			break ;
		if ((game->world.map)[(int)dda->map_pos.y][(int)dda->map_pos.x] == '1')
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
void init_side_dist_and_step(t_player *player, t_dda_ctx *dda)
{
	if (dda->ray.x < 0)
	{
		dda->side_dist.x = dda->delta_dist.x * (player->pos.x - dda->map_pos.x * TILE_SIZE);
		dda->step_dir.x = -1.0;
	}
	else
	{
		dda->side_dist.x = dda->delta_dist.x * ((dda->map_pos.x + 1) * TILE_SIZE - player->pos.x);
		dda->step_dir.x = 1.0;
	}
	if (dda->ray.y < 0)
	{
		dda->side_dist.y = dda->delta_dist.y * (player->pos.y - dda->map_pos.y * TILE_SIZE);
		dda->step_dir.y = -1.0;
	}
	else
	{
		dda->side_dist.y = dda->delta_dist.y * ((dda->map_pos.y + 1) * TILE_SIZE - player->pos.y);
		dda->step_dir.y = 1.0;
	}
}

/**
 * init_map_pos - set the position of player in the grip map
 *
 * @player: pointer to player
 * @dda: pointer to dda algorithm context
 */
void dda_init_map_pos(t_player *player, t_dda_ctx *dda)
{
	dda->map_pos.x = (int) (player->pos.x / TILE_SIZE);
	dda->map_pos.y = (int) (player->pos.y / TILE_SIZE);
	// vec2_print(dda->map_pos, "MAP POSITION");
}

/**
 * compute_ray_dir - calculates ray direction based on player direction and
 * calculate current camera plane slice
 *
 * @game: pointer to game context
 * @dda: pointer to dda algorithm context
 * @x: current screen column
 */
void compute_ray_dir(t_game *game, t_dda_ctx *dda, double x)
{
	dda->ray_size = 2.0 * x / game->screen_width - 1;
	dda->ray = vec2_add(game->player.dir, vec2_scale(game->player.plane, dda->ray_size));
}

/**
 * compute_dist_to_hit_wall - compute perpendicular distance to the hit wall
 *
 * @dda: pointer to dda algorithm context
 */
void compute_dist_to_hit_wall(t_dda_ctx *dda)
{
	if(dda->side == 0)
		dda->hit_dist = (dda->side_dist.x  / TILE_SIZE - dda->delta_dist.x);
	else
		dda->hit_dist = (dda->side_dist.y  / TILE_SIZE - dda->delta_dist.y);
}

/**
 * compute_wall_boundaries - calculate wall slice screen coordinates
 *
 * @game: pointer to game context
 * @dda: pointer to dda algorithm context
 * @x: current screen column
 */
void calculate_wall_boundaries(t_game *game, t_dda_ctx *dda, int x)
{
	double line_height;
	double h;

	line_height = game->screen_height / dda->hit_dist * TILE_SIZE;
	h = game->screen_height;
	dda->line_start.y = (h - line_height) / 2;
	dda->line_end.y = (line_height + h) / 2;
	dda->line_start.x = x;
	dda->line_end.x = x;
	dda->line_height = line_height;
}

/**
 * perform_dda - run all DDA initialization and stepping
 *
 * @game: pointer to game context
 * @dda: pointer to dda algorithm context
 * @x: screen column
 *
 * High-level function to compute ray, run DDA, and calculate hit distance, for
 * every time this function runs, you'll find all the necessary info in the dda
 * context, changing the x, means changing the current column
 */
void perform_dda(t_game *game, t_dda_ctx *dda, int x)
{
	dda_init_map_pos(&game->player, dda);
	compute_ray_dir(game, dda, x);
	init_delta_dist(dda);
	init_side_dist_and_step(&game->player, dda);
	find_first_hitting_wall(game, dda);
	compute_dist_to_hit_wall(dda);
}


void raycast_draw_walls(t_game *game)
{
	t_dda_ctx dda;

	for (int x = 0; x < (int)game->screen_width; x++)
	{
		perform_dda(game, &dda, x);
		if ((int)dda.hit_dist != 0)
		{
			calculate_wall_boundaries(game, &dda, x);
			draw_fov_in_minimap(game, &dda);
			draw_texture_line(game, &dda);
		}
	}
}
