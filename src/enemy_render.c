/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:33:26 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 16:39:43 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * enemy_draw_borders - draw enemy borders to show its boundaries
 */
void	enemy_draw_borders(t_game *game, t_enemy *enemy)
{
	t_vec2	p1;
	t_vec2	p2;
	t_vec2	p3;
	t_vec2	p4;

	p1 = vec2_new(enemy->s.x, enemy->s.y);
	p2 = vec2_new(enemy->e.x, enemy->s.y);
	p3 = vec2_new(enemy->s.x, enemy->e.y);
	p4 = vec2_new(enemy->e.x, enemy->e.y);
	draw_line(&game->scene, p1, p2, COLOR_RED);
	draw_line(&game->scene, p1, p3, COLOR_RED);
	draw_line(&game->scene, p4, p2, COLOR_RED);
	draw_line(&game->scene, p4, p3, COLOR_RED);
}

/**
 * enemy_draw_one - Calculate the enemy posistion in the screen then draw its
 * current frame
 */
void	enemy_draw_one(t_game *game, t_enemy *enemy)
{
	int	w;
	int	h;

	w = game->screen_width;
	h = game->screen_height;
	enemy->camera = enemy_camera_pos(game, enemy);
	enemy->screen = (int)(((double)w / 2) * (1 + enemy->camera.x
				/ enemy->camera.y));
	enemy->size = abs((int)(h / (enemy->camera.y)));
	enemy->s = enemy_get_drawing_start(game, enemy);
	enemy->e = enemy_get_drawing_end(game, enemy);
	if (enemy->camera.y < 0 || enemy->size > 1500)
		return ;
	if (enemy->screen > w + enemy->size)
		return ;
	enemy_draw_frame(game, enemy);
}

/**
 * enemy_update_frame_all - Update the current frame of each enemy in the game
 */
void	enemy_update_frame_all(t_game *game)
{
	t_enemy	*enemy;
	size_t	i;

	i = 0;
	while (i < game->enemies->length)
	{
		enemy = dyn_at(game->enemies, i);
		animation_cycle(game, enemy->animation[enemy->state]);
		i++;
	}
}

/**
 * enemy_draw_all - draw the current frame of each enemy in the game
 */
void	enemy_draw_all(t_game *game)
{
	size_t	i;

	i = 0;
	while (i < game->enemies->length)
	{
		enemy_draw_one(game, dyn_at(game->enemies, i++));
	}
}

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
