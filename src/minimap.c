/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 10:29:11 by aljbari           #+#    #+#             */
/*   Updated: 2025/11/17 22:53:04 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/cub3d.h"

#define MINIMAP_SCALE 4.0
void minimap_draw_player(t_game *game)
{
	double scale = TILE_SIZE / MINIMAP_SCALE;
	t_vec2 head;
	t_vec2 mini_player_pos;


	mini_player_pos = vec2_div(game->player.pos, MINIMAP_SCALE);
	head = vec2_add(vec2_scale(game->player.dir, scale), mini_player_pos);
	draw_line(&game->scene, mini_player_pos, head, COLOR_MINDARO);
	draw_filled_circle(&game->scene, mini_player_pos, 2, COLOR_WHITE);
}

void minimap_draw_enemy(t_game *game)
{
	// double scale = TILE_SIZE / MINIMAP_SCALE;
	t_vec2 mini_enemy_pos;

	mini_enemy_pos = vec2_div(game->enemy.pos, MINIMAP_SCALE);
	draw_filled_circle(&game->scene, mini_enemy_pos, 5, COLOR_PURPLE);
	draw_circle(&game->scene, mini_enemy_pos, 16, COLOR_PURPLE);
}

void draw_fov_in_minimap(t_game *game, t_dda_ctx *dda)
{
	t_vec2 mini_player_pos; // the player position in the minimap
	t_vec2 mini_hit_point;  // where the ray hits in minimap?

	mini_player_pos = vec2_div(game->player.pos, MINIMAP_SCALE);
	mini_hit_point = vec2_add(mini_player_pos, 
			   vec2_scale(dda->ray, dda->hit_dist / MINIMAP_SCALE)
			   );
	draw_line(&game->scene, 
	   mini_player_pos, 
	   mini_hit_point
	   , COLOR_MINDARO);
}

void minimap_draw_walls(t_game *game)
{
	size_t i = 0;
	size_t j;
	int grid_sz;

	grid_sz = TILE_SIZE / MINIMAP_SCALE;
	while (i < game->world.map_height)
	{
		j = 0;
		while (j < game->world.map_width)
		{
			if (game->world.map[i][j] == '1')
				draw_filled_square(&game->scene, vec2_new(j * grid_sz, i * grid_sz), grid_sz, COLOR_GREEN);
			j++;
		}
		i++;
	}
}

void minimap_daw_grid(t_game *game)
{
	t_data *buff;
	t_vec2 start;
	size_t i;
	int grid_sz;

	grid_sz = TILE_SIZE / MINIMAP_SCALE;
	buff = &game->scene;

	start = vec2_new(0, 0);
	i = 0;
	while (i <= game->world.map_height)
	{
		draw_horizontal_line(buff, start, game->world.map_width * grid_sz, COLOR_RED);
		start.y += grid_sz;
		i++;
	}
	start = vec2_new(0, 0);
	i = 0;
	while (i <= game->world.map_width)
	{
		draw_vertical_line(buff, start, game->world.map_height * grid_sz, COLOR_RED);
		start.x += grid_sz;
		i++;
	}
}

void draw_minimap(t_game *game)
{
	minimap_draw_walls(game);
	minimap_daw_grid(game);
	minimap_draw_player(game);
	minimap_draw_enemy(game);
}
