/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:54:38 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/13 21:54:41 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include <mlx.h>

t_parse	*box(void)
{
	static t_parse var;

	return	(&var);
}

int map[MAP_HEIGHT][MAP_WIDTH] = {
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,0,0,0,0,6,0,0,0,1},
  {1,0,0,0,0,0,0,0,0,1},
  {1,1,1,1,1,1,1,1,1,1}
};

void	init(void)
{
	int	i;
	int	j;

	i = 0;
	while (i < MAP_HEIGHT)
	{
		j = 0;
		while (j < MAP_WIDTH)
		{
			if (map[i][j] == 6)
			{
				box()->px = i;
				box()->py = j;
			}
			j++;
		}
		i++;
	}
	box()->height = i;
	box()->width = j;
}

void	render_wall(int x, int y, void *win, void *mlx, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < 43)
	{
		j = 0;
		while (j < 43)
		{
			mlx_pixel_put(mlx, win, y + i, x + j, color);
			j++;
		}
		i++;
	}
}

void	render_player(int x, int y, void *win, void *mlx, int color)
{
	int	i;
	int	j;

	i = 0;
	while (i < 5)
	{
		j = 0;
		while (j < 5)
		{
			if ((y - 3 + i + 1) % 45 != 0 && (x - 3 + j + 1) % 45 != 0
				&& (y - 3 + i + 2) % 45 != 0 && (x - 3 + j + 2) % 45 != 0)
				mlx_pixel_put(mlx, win, y - 3 + i, x - 3 + j, color);
			j++;
		}
		i++;
	}
}

void	draw_walls(t_parse *ptr, void *mlx, void *win)
{
	int	i;
	int	j;

	i = 0;
	while (i < ptr->height)
	{
		j = 0;
		while (j < ptr->width)
		{
			if (map[i][j] == 1)
				render_wall(i * 45, j * 45, win, mlx, 0x555555);
			else if (map[i][j] == 6)
			{
				box()->px = j * 45 + 22;
				box()->py = i * 45 + 22;
				render_wall(i * 45, j * 45, win, mlx, 0x808080);
			}
			else
				render_wall(i * 45, j * 45, win, mlx, 0x808080);
			j++;
		}
		i++;
	}
}

int	function_handle_(void)
{
	static int oldx_;
	static int oldy_;

	if (oldy_ != box()->py || oldx_ != box()->px)
	{
		printf("py %d, px %d\n", box()->py, box()->px);
		if (oldy_ || oldx_)
			render_player(oldx_, oldy_ , box()->win, box()->mlx, 0x808080);
		render_player(box()->px , box()->py , box()->win, box()->mlx, 0xB32134);
		oldx_ = box()->px;
		oldy_ = box()->py;
	}
	return (0);
}

int	escape_handle(int event_c_)
{
	if (event_c_ == KE)
		exit(0);
	else if (event_c_ == KA)
		box()->py -= 2;
	else if (event_c_ == KS)
		box()->py += 2;
	else if (event_c_ == KD)
		box()->px += 2;
	else if (event_c_ == KW)
		box()->px -= 2;
	return 0;
}

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	init();
	box()->mlx = mlx_init();
	box()->win = mlx_new_window(box()->mlx, box()->height*45, box()->width*45, "Cub3d test");
	draw_walls(box(), box()->mlx, box()->win);
	mlx_hook(box()->win, 02, 1L<<0, escape_handle, NULL);
	mlx_key_hook(box()->win, function_handle_, NULL);
	mlx_loop(box()->mlx);
	return (0);
}
