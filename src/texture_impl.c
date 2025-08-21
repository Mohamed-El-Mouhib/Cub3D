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
 * hit_point - function exetract the hitpoint from other informations
 * return:	the first intersection of the wall 
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
 * get_color_info - function calcuate the the vecture of the right color depend
 *	on vectore of first hit of the wall
 * @vec:	the vectore of the pixel color on the texture
 * @offs:	the color vector on the texture
 * @wallh:	wall height
*/
unsigned int get_color_info(t_game *game, t_dda_ctx *info, int i)
{
	t_vec2	vec;
	int	offs;
	double	wallh;

	wallh = (double)(info->line_end.y - info->line_start.y);
	vec.x = fmod(hit_points(game, info), 1.0) * game->frames.Dimensions[info->side][0];
	if (vec.x < 0)
		vec.x = 0;
	else if (vec.x >= game->frames.Dimensions[info->side][0])
		vec.x = game->frames.Dimensions[info->side][0] - 1;
	vec.y = (i / wallh) * game->frames.Dimensions[info->side][1];
	if (vec.y < 0)
		vec.y = 0;
	else if (vec.y >= game->frames.Dimensions[info->side][1])
		vec.y = game->frames.Dimensions[info->side][1] - 1;
	offs = (int)vec.y * game->frames.image[info->side].line_len + (int)vec.x * (game->frames.image[info->side].bpp / 8);
	return *(unsigned int *)(game->frames.walltex_[info->side] + offs);
}
/**
 * draw_texture_line - testing is everything how can I help you today?
 * @i: iterator for drawing ceiling, wall and floor
 * 	while i is still less than the start of the line it goona set the pixel to ceiling color,
 * 	and if the i is in the range of the line_start < i <line_end it gonna set the pixel using get_color_info
 * 	and the opposite of the first instruct while it's outside of the wall range the floor color
 * @px: stores the color of the current pixel that gonna sent to image
 *
 */
void	draw_texture_line(t_game *game, t_dda_ctx *info)
{
	size_t		i;
	unsigned int	px;

	i = 0;
	while (i < game->screen_height)
	{
		if (i < info->line_start.y)
			px = COLOR_BLUE;
		else if (i > info->line_end.y)
			px = COLOR_NYANZA;
		else
			px = get_color_info(game, info, i - info->line_start.y);
		image_put_pixel(&game->scene,
		info->line_start.x, i, px);
		i++;
	}
}
