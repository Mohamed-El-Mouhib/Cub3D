/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_impl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: your_login <your_login@student.1337.ma>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:49:22 by your_login        #+#    #+#             */
/*   Updated: 2025/08/20 11:50:43 by your_login       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * hit_point - Extracts coordinate of the wall hit point.
 * @hit:   Vector of the hit point. For a horizontal wall, use the x coordinate.
 *         For a vertical wall, use the y coordinate. Both are normalized by TILE_SIZE.
 * return: The intersection point on the wall.
*/
double	hit_points(t_game *game, t_dda_ctx *info)
{
	t_vec2	hit;

	hit = vec2_add(game->player.pos, vec2_scale(info->ray, info->hit_dist));
	if (info->side)
		return (hit.x/TILE_SIZE);
	else
		return (hit.y/TILE_SIZE);
}

/**
 * get_color_info - Calculates the texture coordinates for the correct color.
 * @vec:   The vector representing the texture coordinates (x, y).
 *         - x: Calculated from the fractional part of the wall hit position,
 *              scaled by the texture width.
 *         - y: Calculated from the current row on the wall, divided by wall height,
 *              then scaled by the texture height.
 *         Both x and y are clamped to the valid range [0, texture size - 1].
 * @offs:  The color offset in the texture.
 * @i:	   The current row the wall.
 * @wallh: The height of the wall.
 *
 * This function ensures the texture coordinates are within bounds and
 * returns the correct color vector from the texture map.
 */
unsigned int get_color_info(t_game *game, t_dda_ctx *info, int i, t_vec2 *vec)
{
	int	offs;

	vec->y = (double)(i / info->line_height) * game->wall[info->side].height;
	if (vec->y < 0)
		vec->y = 0;
	else if (vec->y >= game->wall[info->side].height)
		vec->y = game->wall[info->side].height - 1;

	offs = (int)vec->y * game->wall[info->side].line_len + (int)vec->x * (game->wall[info->side].bpp / 8);
	return *(unsigned int *)(game->wall[info->side].addr + offs);
}

typedef struct s_color {
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} t_color_;

/**
 * draw_texture_line - Draws a vertical line with ceiling, wall, and floor colors.
 * @i: Current pixel position in the column.
 *    - If i < line_start: set pixel to ceiling color.
 *    - If line_start <= i < line_end: set pixel using get_color_info (wall).
 *    - If i >= line_end: set pixel to floor color.
 * @px: Color value to be set for the current pixel.
 *
 */
// Adjustable constant: Higher value = Fog starts closer
#define FOG_INTENSITY 25.0 

static unsigned int apply_fog(unsigned int color_val, double factor)
{
	t_color_ *c;

	if (factor == 0)
		return color_val;
	c = (t_color_*)&color_val;
	c->r /= factor;
	c->g /= factor;
	c->b /= factor;
	return (*(unsigned int *)c);
}

void	draw_texture_line(t_game *game, t_dda_ctx *info)
{
	int		i;
	t_vec2		vec;
	unsigned int	px;
	double factor;

	i = -1;
	vec.x = fmod(hit_points(game, info), 1.0) * game->wall[info->side].width;
	if (vec.x < 0)
		vec.x = 0;
	else if (vec.x >= game->wall[info->side].width)
		vec.x = game->wall[info->side].width - 1;
	while (++i < (int)game->screen_height)
	{
		if (i < info->line_start.y)
		{
			factor  = lerp(1, 6, i / (double)game->screen_height);
			px = game->ceiling; // this supposed to be ceiling
		}
		else if (i > info->line_end.y)
		{
			factor  = lerp(6, 1, i / (double)game->screen_height);
			px = game->floor; // this supposed to be ceiling
		}
		else
		{
			if (info->hit_dist > 95)
				factor = info->hit_dist / 95;
			else
				factor = 1;
			px = get_color_info(game, info, i - info->line_start.y, &vec);
		}
		px = apply_fog(px, factor);
		image_put_pixel(&game->scene, info->line_start.x, i, px);
	}
}
