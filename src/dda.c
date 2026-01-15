/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 10:26:43 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 17:31:12 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * compute_ray_dir - calculates ray direction based on player direction and
 * calculate current camera plane slice
 *
 * @game: pointer to game context
 * @dda: pointer to dda algorithm context
 * @x: current screen column
 */
void	compute_ray_dir(t_game *game, t_dda_ctx *dda, double x)
{
	dda->ray_size = 2.0 * x / game->screen_width - 1;
	dda->ray = vec2_add(game->player.dir, vec2_scale(game->player.plane,
				dda->ray_size));
}

/**
 * compute_dist_to_hit_wall - compute perpendicular distance to the hit wall
 *
 * @dda: pointer to dda algorithm context
 */
void	compute_dist_to_hit_wall(t_dda_ctx *dda)
{
	if (dda->side == WALL_NORTH || dda->side == WALL_SOUTH)
		dda->hit_dist = (dda->side_dist.y / TILE_SIZE - dda->delta_dist.y);
	else
		dda->hit_dist = (dda->side_dist.x / TILE_SIZE - dda->delta_dist.x);
	if (dda->hit_dist < 1)
		dda->hit_dist = 1;
}

/**
 * compute_wall_boundaries - calculate wall slice screen coordinates
 *
 * @game: pointer to game context
 * @dda: pointer to dda algorithm context
 * @x: current screen column
 */
void	calculate_wall_boundaries(t_game *game, t_dda_ctx *dda, int x)
{
	double	line_height;
	double	h;
	double	shake;

	shake = 0;
	line_height = game->screen_height / dda->hit_dist * TILE_SIZE;
	h = game->screen_height;
	if (game->shake > 0)
		shake = cos(game->tick / 20.0) * game->shake;
	dda->line_start.y = (h - line_height) / 2 + shake;
	dda->line_end.y = (line_height + h) / 2 + shake;
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
void	perform_dda(t_game *game, t_dda_ctx *dda, int x)
{
	dda_init_map_pos(&game->player, dda);
	compute_ray_dir(game, dda, x);
	init_delta_dist(dda);
	init_side_dist_and_step(&game->player, dda);
	find_first_hitting_wall(game, dda);
	compute_dist_to_hit_wall(dda);
}

void	raycast_draw_walls(t_game *game)
{
	t_dda_ctx	dda;
	int			x;

	x = 0;
	while (x < (int)game->screen_width)
	{
		perform_dda(game, &dda, x);
		game->stripes[x] = dda.hit_dist;
		if ((int)dda.hit_dist != 0)
		{
			calculate_wall_boundaries(game, &dda, x);
			draw_texture_line(game, &dda, -1);
		}
		x++;
	}
}
