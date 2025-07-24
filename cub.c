/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:54:38 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/24 23:57:29 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include <mlx.h>

t_parse	*box(void)
{
	static t_parse var;

	return	(&var);
}

t_movment	*pmo(void)
{
	static t_movment pp;

	return (&pp);
}

int map[MAP_HEIGHT][MAP_WIDTH] = {
  {1,1,1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,0,0,1,1},
  {1,0,0,1,0,0,0,0,0,1},
  {1,0,0,0,1,1,1,0,0,1},
  {1,0,0,0,0,1,1,0,0,1},
  {1,0,0,1,0,6,1,0,0,1},
  {1,0,0,1,1,0,0,0,0,1},
  {1,0,0,1,1,1,0,0,0,1},
  {1,1,0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1,1,1}
};

void	init(void)
{
	int	i;
	int	j;

	i = -1;
	while (++i < MAP_HEIGHT)
	{
		j = -1;
		while (++j < MAP_WIDTH)
		{
			if (map[i][j] == 6)
			{
				box()->px = (double)i *45 + 22;
				box()->py = (double)j *45 + 22;
			}
		} }
	box()->height = i;
	box()->width = j;
}

void	render_wall(int x, int y, int color)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 43)
	{
		j = -1;
		while (++j < 43)
			my_pixel_put(x + i, y + j, color);
	}
}

void	my_pixel_put(int x, int y, unsigned int color)
{
	unsigned int	offset;

	offset = (y * box()->data.line_len + x * (box()->data.bpp / 8));
	*((unsigned int *)(box()->data.addr + offset)) = color;
}

void	render_player(int x, int y, int color)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 5)
	{
		j = -1;
		while (++j < 5)
		{
			if ((y - 3 + i + 1) % 45 != 0 && (x - 3 + j + 1) % 45 != 0
				&& (y - 3 + i + 2) % 45 != 0 && (x - 3 + j + 2) % 45 != 0)
				my_pixel_put(y - 3 + i, x - 3 + j, color);
		}
	}
}

void	draw_walls(void)
{
	int	i;
	int	j;

	i = -1;
	while (++i < box()->height)
	{
		j = -1;
		while (++j < box()->width)
		{
			if (map[i][j] == 1)
				render_wall(i * 45, j * 45, 0x555555);
			else
				render_wall(i * 45, j * 45, 0x808080);
		}
	}
}

int	function_handle_(void)
{
	static double	oldy_;
	static double	oldx_;

	usleep(1000);
	draw_walls();
	if (map[(int)(box()->px + pmo()->left_right_) / 45][(int)box()->py / 45] != 1)
		box()->py += pmo()->left_right_;
	box()->px += pmo()->up_down_;
	printf("py %d, px %d\n", (int)box()->py, (int)box()->px);
	if (oldy_ || oldx_)
		render_player((int)oldx_, (int)oldy_, 0x808080);
	render_player((int)box()->px, (int)box()->py, 0xB32134);
	oldx_ = box()->px;
	oldy_ = box()->py;
	mlx_put_image_to_window(box()->mlx, box()->win, box()->data.img, 0, 0);
	return (0);
}

int	key_set_(int event_c_, double *mode_)
{
	if (event_c_ == XK_Escape)
		exit(0);
	else if (event_c_ == XK_a)
		pmo()->left_right_ = -(*mode_);
	else if (event_c_ == XK_s)
		pmo()->up_down_ = (*mode_);
	else if (event_c_ == XK_d)
		pmo()->left_right_ = (*mode_);
	else if (event_c_ == XK_w)
		pmo()->up_down_ = -(*mode_);
	return 0;
}

int	main(int ac, char **av)
{
	double	set_flag;
	double	unset_flag;

	set_flag = 0.2;
	unset_flag = 0;
	pmo()->is_moving = true;
	(void)ac;
	(void)av;
	init();
	box()->mlx = mlx_init();
	box()->win = mlx_new_window(box()->mlx, box()->height*45, box()->width*45, "Cub3d test");
	box()->data.img = mlx_new_image(box()->mlx, box()->height * 45, box()->width * 45);
	box()->data.addr = mlx_get_data_addr(box()->data.img, &box()->data.bpp, &box()->data.line_len, &box()->data.endian);
	mlx_hook(box()->win, 02, 1L<<0, key_set_, &set_flag);
	mlx_hook(box()->win, 03, 1L<<1, key_set_, &unset_flag);
	mlx_loop_hook(box()->mlx, function_handle_, NULL);
	mlx_loop(box()->mlx);
	return (0);
}
