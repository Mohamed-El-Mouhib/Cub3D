/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 10:29:11 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 16:02:37 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/cub3d.h"

void	draw_fov_in_minimap(t_game *game, t_dda_ctx *dda);
int		calc_distance(int x, int y);
void	minimap_draw_player(t_game *game);

void	minimap_draw_walls(t_game *game)
{
	int	i;
	int	j;
	int	grid_sz;

	i = 0;
	grid_sz = TILE_SIZE / MINIMAP_SCALE;
	while (i < game->world.map_height)
	{
		j = 0;
		while (j < game->world.map_width)
		{
			if (game->world.map[i][j] == '1')
				draw_filled_square(&game->scene, vec2_new(j * grid_sz, i
						* grid_sz), grid_sz, COLOR_GREEN);
			else if (game->world.map[i][j] == 'C')
				draw_filled_square(&game->scene, vec2_new(j * grid_sz, i
						* grid_sz), grid_sz, COLOR_RED);
			j++;
		}
		i++;
	}
}

void	minimap_daw_grid(t_game *game)
{
	t_data	*buff;
	t_vec2	start;
	int		i;
	int		grid_sz;

	grid_sz = TILE_SIZE / MINIMAP_SCALE;
	buff = &game->scene;
	start = vec2_new(0, 0);
	i = 0;
	while (i <= game->world.map_height)
	{
		draw_horizontal_line(buff, start, game->world.map_width * grid_sz,
			COLOR_RED);
		start.y += grid_sz;
		i++;
	}
	start = vec2_new(0, 0);
	i = 0;
	while (i <= game->world.map_width)
	{
		draw_vertical_line(buff, start, game->world.map_height * grid_sz,
			COLOR_RED);
		start.x += grid_sz;
		i++;
	}
}

void	calc_minimap_tiles(t_game *game, int i, int j)
{
	int		p_x;
	int		p_y;
	int		t_y;
	int		t_x;
	char	tile;

	p_x = game->player.pos.x - (HALF_MINIMAP * MINIMAP_ZOOM) + (i
			* MINIMAP_ZOOM);
	p_y = game->player.pos.y - (HALF_MINIMAP * MINIMAP_ZOOM) + (j
			* MINIMAP_ZOOM);
	t_x = p_x / TILE_SIZE;
	t_y = p_y / TILE_SIZE;
	if (p_x < 0 || p_y < 0 || t_x < 0 || t_y < 0
		|| game->world.map_height <= t_y || game->world.map_width <= t_x)
	{
		image_put_pixel(&game->scene, i + X_OFFSET, j + Y_OFFSET, 0x6e6e6e);
		return ;
	}
	tile = game->world.map[t_y][t_x];
	if (tile == '1' || tile == 'C')
		image_put_pixel(&game->scene, i + X_OFFSET, j + Y_OFFSET, 0x8a8a8a);
	else
		image_put_pixel(&game->scene, i + X_OFFSET, j + Y_OFFSET, 0x5c5c5c);
}

void	draw_circle_map(t_game *game)
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
				image_put_pixel(&game->scene, i + X_OFFSET, j + Y_OFFSET,
					0x545454);
			j++;
		}
		i++;
	}
}

void	draw_minimap(t_game *game)
{
	draw_circle_map(game);
	minimap_draw_player(game);
}
