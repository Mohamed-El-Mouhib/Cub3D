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

#include "cub3d.h"

/**
 * enemy_camera_pos - Calculate the position of the player in the coordinate
 * system: (player->dir, player->plane)
 */
t_vec2	enemy_camera_pos(t_game *game, t_enemy *enemy)
{
	t_player	*p;
	t_vec2		camera_pos;
	t_vec2		p_to_enemy;
	double		invdet;

	p = &game->player;
	p_to_enemy = vec2_scale(vec2_sub(enemy->pos, p->pos), 1 / TILE_SIZE);
	invdet = 1.0 / (p->plane.x * p->dir.y - p->dir.x * p->plane.y);
	camera_pos.x = invdet * (p->dir.y * p_to_enemy.x - p->dir.x * p_to_enemy.y);
	camera_pos.y = invdet * (-p->plane.y * p_to_enemy.x + p->plane.x
			* p_to_enemy.y);
	return (camera_pos);
}

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

/**
 * enemy_draw_frame - draw the current frame of the enemy in the correct
 * position
 */
void	enemy_draw_frame(t_game *game, t_enemy *enemy)
{
	t_color	color;
	t_color	ignore_color;
	t_data	*frame;

	frame = animation_get_frame(game, enemy->animation[enemy->state]);
	ignore_color = *(unsigned int *)frame->addr;
	for (int x = enemy->s.x; x < enemy->e.x; x++)
	{
		if (x < 0 || enemy->camera.y > game->stripes[x] / TILE_SIZE)
			continue ;
		for (int y = enemy->s.y; y < enemy->e.y; y++)
		{
			if (y < 0)
				continue ;
			color = get_point_color(game, enemy, x, y);
			if (ignore_color == color)
				continue ;
			image_put_pixel(&game->scene, x, y, color);
		}
	}
}
