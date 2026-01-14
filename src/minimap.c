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

int	calc_distance(int x, int y)
{
	return (x * x + y * y);
}

void minimap_draw_player(t_game *game)
{
	double scale = TILE_SIZE / MINIMAP_SCALE;
	t_vec2 head;
	t_vec2 mini_player_pos;


	mini_player_pos = vec2_new(HALF_MINIMAP + X_OFFSET, HALF_MINIMAP + Y_OFFSET);
	head = vec2_add(vec2_scale(game->player.dir, scale), mini_player_pos);
	draw_line(&game->scene, mini_player_pos, head, COLOR_WHITE);
	draw_filled_circle(&game->scene, mini_player_pos, 5, COLOR_WHITE);
	// minimap_draw_enemy(game, mini_player_pos);
}

void draw_fov_in_minimap(t_game *game, t_dda_ctx *dda)
{
	t_vec2 mini_player_pos; // the player position in the minimap
	t_vec2 mini_hit_point;  // where the ray hits in minimap?

	mini_player_pos = vec2_new(120, 120);
	// mini_player_pos = vec2_div(game->player.pos, MINIMAP_SCALE);
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
	int i = 0;
	int j;
	int grid_sz;

	grid_sz = TILE_SIZE / MINIMAP_SCALE;
	while (i < game->world.map_height)
	{
		j = 0;
		while (j < game->world.map_width)
		{
			if (game->world.map[i][j] == '1')
				draw_filled_square(&game->scene, vec2_new(j * grid_sz, i * grid_sz), grid_sz, COLOR_GREEN);
			else if (game->world.map[i][j] == 'C')
				draw_filled_square(&game->scene, vec2_new(j * grid_sz, i * grid_sz), grid_sz, COLOR_RED);
			j++;
		}
		i++;
	}
}

void minimap_daw_grid(t_game *game)
{
	t_data *buff;
	t_vec2 start;
	int i;
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

void	calc_minimap_tiles(t_game* game, int i, int j)
{
	int	p_x;
	int	p_y;
	int	t_y;
	int	t_x;
	char	tile;

	p_x = game->player.pos.x - (HALF_MINIMAP * MINIMAP_ZOOM) + (i * MINIMAP_ZOOM) ;
	p_y = game->player.pos.y - (HALF_MINIMAP * MINIMAP_ZOOM) + (j * MINIMAP_ZOOM) ;
	t_x = p_x / TILE_SIZE;
	t_y = p_y / TILE_SIZE;
	if (p_x < 0 || p_y < 0 || t_x < 0 || t_y < 0
		|| game->world.map_height <= t_y || game->world.map_width <= t_x)
	{
		image_put_pixel(&game->scene, i + X_OFFSET, j + Y_OFFSET, 0x6e6e6e);
		return;
	}
	tile = game->world.map[t_y][t_x];
	if (tile == '1' || tile == 'C')
		image_put_pixel(&game->scene, i + X_OFFSET, j + Y_OFFSET, 0x8a8a8a);
	else
		image_put_pixel(&game->scene, i + X_OFFSET, j + Y_OFFSET, 0xB1B1B1);
}

void	draw_circle_map(t_game* game)
{
	int	i;
	int	j;
	int	range;

	i = 0;
	while (i < MINIMAP_LEN)
	{
		j = 0;
		while (j < MINIMAP_LEN)
		{
			range = calc_distance(i - HALF_MINIMAP, j - HALF_MINIMAP);
			if (range <= SQR_REDIOUS - 1000)
				calc_minimap_tiles(game, i, j);
			else if (range < SQR_REDIOUS)
				image_put_pixel(&game->scene, i+ X_OFFSET, j + Y_OFFSET, 0x545454);
			j++;
		}
		i++;
	}
}

void draw_minimap(t_game *game)
{
	draw_circle_map(game);
	minimap_draw_player(game);
}
