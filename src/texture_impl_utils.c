/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_impl_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 17:11:44 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 17:13:53 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * hit_point - Extracts coordinate of the wall hit point.
 * @hit:   Vector of the hit point. For a horizontal wall, use the x coordinate.
 *         For a vertical wall,
	use the y coordinate. Both are normalized by TILE_SIZE.
 * return: The intersection point on the wall.
 */
double	get_side_and_cords(t_game *game, t_dda_ctx *info)
{
	t_vec2	hit;

	hit = vec2_add(game->player.pos, vec2_scale(info->ray, info->hit_dist));
	if (info->side == WALL_NORTH)
		return (TILE_SIZE - (hit.x / TILE_SIZE));
	if (info->side == WALL_SOUTH)
		return (hit.x / TILE_SIZE);
	if (info->side == WALL_WEST)
		return (hit.y / TILE_SIZE);
	else
		return (TILE_SIZE - (hit.y / TILE_SIZE));
}

/**
 * get_color_info - Calculates the texture coordinates for the correct color.
 * @vec:   The vector representing the texture coordinates (x, y).
 *         - x: Calculated from the fractional part of the wall hit position,
 *              scaled by the texture width.
 *         - y: Calculated from the current row on the wall,
	divided by wall height,
 *              then scaled by the texture height.
 *         Both x and y are clamped to the valid range [0, texture size - 1].
 * @offs:  The color offset in the texture.
 * @i:		The current row the wall.
 * @wallh: The height of the wall.
 *
 * This function ensures the texture coordinates are within bounds and
 * returns the correct color vector from the texture map.
 */

unsigned int	get_color_info(t_data *side, t_dda_ctx *info, int i,
		t_vec2 *vec)
{
	vec->y = (double)(i / info->line_height) * side->height;
	if (vec->y < 0)
		vec->y = 0;
	else if (vec->y >= side->height)
		vec->y = side->height - 1;
	return (image_get_pixel(side, (int)vec->x, (int)vec->y));
}

/**
 * draw_texture_line - Draws a vertical line with ceiling, wall,
	and floor colors.
 * @i: Current pixel position in the column.
 *    - If i < line_start: set pixel to ceiling color.
 *    - If line_start <= i < line_end: set pixel using get_color_info (wall).
 *    - If i >= line_end: set pixel to floor color.
 * @px: Color value to be set for the current pixel.
 *
 */
// Adjustable constant: Higher value = Fog starts closer
#define FOG_INTENSITY 25.0

unsigned int	apply_fog(unsigned int color_val, double factor)
{
	t_color_	*c;

	if (factor == 0)
		return (color_val);
	c = (t_color_ *)&color_val;
	c->r /= factor;
	c->g /= factor;
	c->b /= factor;
	return (*(unsigned int *)c);
}

double	get_texture_x(t_game *game, t_dda_ctx *info)
{
	double	x;

	x = fmod(get_side_and_cords(game, info), 1.0)
		* game->wall[info->side].width;
	if (x < 0)
		x = 0;
	else if (x >= game->wall[info->side].width)
		x = game->wall[info->side].width - 1;
	return (x);
}

double	get_texture_y(t_game *game, t_dda_ctx *info, int i)
{
	double	y;

	y = (double)(i / info->line_height) * game->wall[info->side].height;
	if (y < 0)
		y = 0;
	else if (y >= game->wall[info->side].height)
		y = game->wall[info->side].height - 1;
	return (y);
}