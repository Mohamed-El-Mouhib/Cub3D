/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:06:09 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 16:06:09 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	calc_distance(int x, int y)
{
	return (x * x + y * y);
}

void	minimap_draw_player(t_game *game)
{
	double	scale;
	t_vec2	head;
	t_vec2	mini_player_pos;

	scale = TILE_SIZE / MINIMAP_SCALE;
	mini_player_pos = vec2_new(HALF_MINIMAP + X_OFFSET, HALF_MINIMAP
			+ Y_OFFSET);
	head = vec2_add(vec2_scale(game->player.dir, scale), mini_player_pos);
	draw_line(&game->scene, mini_player_pos, head, COLOR_WHITE);
	draw_filled_circle(&game->scene, mini_player_pos, 5, COLOR_WHITE);
}

void	draw_fov_in_minimap(t_game *game, t_dda_ctx *dda)
{
	t_vec2	mini_player_pos;
	t_vec2	mini_hit_point;

	mini_player_pos = vec2_new(120, 120);
	mini_hit_point = vec2_add(mini_player_pos, vec2_scale(dda->ray,
				dda->hit_dist / MINIMAP_SCALE));
	draw_line(&game->scene, mini_player_pos, mini_hit_point, COLOR_MINDARO);
}
