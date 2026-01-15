/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_impl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 11:49:22 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 17:11:27 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#include <stdio.h>


t_data	*init_side(t_game *game, t_dda_ctx *info)
{
	if (game->world.map[(int)info->map_pos.y][(int)info->map_pos.x] == 'C')
		return (&game->wall[WALL_DOOR]);
	else
		return (&game->wall[info->side]);
}

void	draw_texture_line(t_game *game, t_dda_ctx *info)
{
	int				i;
	t_vec2			vec;
	t_data			*side;
	unsigned int	px;
	double			factor;

	i = -1;
	vec.x = get_texture_x(game, info);
	side = init_side(game, info);
	while (++i < (int)game->screen_height)
	{
		if (i < info->line_start.y)
		{
			factor = lerp(1, 6, i / (double)game->screen_height);
			px = game->ceiling;
		}
		else if (i > info->line_end.y)
		{
			factor = lerp(6, 1, i / (double)game->screen_height);
			px = game->floor;
		}
		else if (i < info->line_end.y && i > info->line_start.y)
		{
			if (info->hit_dist > 95)
				factor = info->hit_dist / 95;
			else
				factor = 1;
			vec.y = get_texture_y(game, info, i - info->line_start.y);
			px = get_color_info(side, info, i - info->line_start.y, &vec);
		}
		px = apply_fog(px, factor);
		image_put_pixel(&game->scene, info->line_start.x, i, px);
	}
}
