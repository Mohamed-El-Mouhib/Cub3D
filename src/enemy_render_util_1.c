/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_render_util_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:39:15 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 16:40:39 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 *    --------------------------------
 *    |        This --> -----------  |
 *    |                 |         |  |
 *    |                 |  enemy  |  |
 *    |                 |         |  |
 *    |                 -----------  |
 *    -------------------------------- <- Screen
 *
 * Returns the postistion of the point referred to in the figure
 */
t_vec2	enemy_get_drawing_start(t_game *game, t_enemy *enemy)
{
	t_vec2	start;

	start.x = enemy->screen - (int)(enemy->size / 2);
	start.y = (int)((game->screen_height - enemy->size) / 2);
	return (start);
}

/**
 *    --------------------------------
 *    |  -----------                  |
 *    |  |         |                  |
 *    |  |  Enemy  |                  |
 *    |  |         |                  |
 *    |  ----------- <-- This         |
 *    --------------------------------- <- Screen
 *
 * Return: The postistion of the point referred to in the figure
 */
t_vec2	enemy_get_drawing_end(t_game *game, t_enemy *enemy)
{
	t_vec2	bottom;

	bottom.x = (int)(enemy->size / 2) + enemy->screen;
	if (bottom.x >= (int)game->screen_width)
		bottom.x = game->screen_width - 1;
	bottom.y = (int)((enemy->size + game->screen_height) / 2);
	if (bottom.y >= (int)game->screen_height)
		bottom.y = game->screen_height - 1;
	return (bottom);
}

/**
 * get_point_color - get the pixel color of the point (i, j)
 */
unsigned int	get_point_color(t_game *game, t_enemy *enemy, int i, int j)
{
	int		tex_w;
	int		tex_h;
	int		x;
	int		y;
	t_data	*frame;

	frame = animation_get_frame(game, enemy->animation[enemy->state]);
	tex_w = frame->width;
	tex_h = frame->height;
	x = (i - enemy->s.x) / enemy->size * tex_w;
	y = (j - enemy->s.y) / enemy->size * tex_h;
	return (image_get_pixel(frame, x, y));
}

void	_enemy_draw_stipe(t_game *game, t_enemy *enemy, int x)
{
	t_color	color;
	t_color	ignore_color;
	t_data	*frame;
	int		y;

	frame = animation_get_frame(game, enemy->animation[enemy->state]);
	ignore_color = *(unsigned int *)frame->addr;
	y = enemy->s.y;
	while (y < enemy->e.y)
	{
		if (y >= 0)
		{
			color = get_point_color(game, enemy, x, y);
			if (ignore_color != color)
				image_put_pixel(&game->scene, x, y, color);
		}
		y++;
	}
}

/**
 * enemy_draw_frame - draw the current frame of the enemy in the correct
 * position
 */
void	enemy_draw_frame(t_game *game, t_enemy *enemy)
{
	int	x;

	x = enemy->s.x;
	while (x < enemy->e.x)
	{
		if (x > 0 && enemy->camera.y < game->stripes[x] / TILE_SIZE)
			_enemy_draw_stipe(game, enemy, x);
		x++;
	}
}
