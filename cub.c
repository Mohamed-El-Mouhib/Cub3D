/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:54:38 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/26 17:26:04 by mel-mouh         ###   ########.fr       */
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
  {1,1,1,1,1,1},
  {1,0,1,1,0,1},
  {1,0,1,0,0,1},
  {1,0,1,6,0,1},
  {1,0,0,0,0,1},
  {1,1,1,1,1,1},
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
				box()->py = (double)i * 45 + 22;
				box()->px = (double)j * 45 + 22;
			}
		}
	}
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

void	draw_lines()
{
	int	i;
	double	y;
	double	x;

	i = 0;
	y = box()->py;
	x = box()->px;
	while ((int)i < MAX_DISTANCE)
	{
		if (((int)y + 1) % 45 != 0 && ((int)x + 1) % 45 != 0
			&& ((int)y + 2) % 45 != 0 && ((int)x + 2) % 45 != 0)
			my_pixel_put((int)x, (int)y, 0xFFFFFF);
		y = box()->py + cos(box()->dir_) * i;
		x = box()->px + sin(box()->dir_) * i;
		i++;
	}
}

void	render_player(int x, int y, int color)
{
	int	i;
	int	j;

	i = -1;
	while (++i < 9)
	{
		j = -1;
		while (++j < 9)
		{
			if ((!i || i == 8 || !j || j == 8) && (y - 5 + j + 1) % 45 != 0 && (x - 5 + i + 1) % 45 != 0
				&& (y - 5 + j + 2) % 45 != 0 && (x - 5 + i + 2) % 45 != 0)
				my_pixel_put(y - 5 + j, x - 5 + i, color);
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
				render_wall(j * 45, i * 45, 0x555555);
			else
				render_wall(j * 45, i * 45, 0x808080);
		}
	}
}

void	cast_rays(void)
{
	double	i;
	int	j;
	double	y;
	double	x;

	i = box()->dir_ - (PI / 3);
	while (i < box()->dir_ + (PI / 3))
	{
		j = 0;
		while (j < MAX_DISTANCE)
		{
			y = box()->py + cos(i) * j;
			x = box()->px + sin(i) * j;
			if (((int)y + 1) % 45 != 0 && ((int)x + 1) % 45 != 0
				&& ((int)y + 2) % 45 != 0 && ((int)x + 2) % 45 != 0)
				my_pixel_put((int)x, (int)y, 0xFFFFFF);
			j++;
		}
		i += 0.7;
	}
}

int	function_handle_(void)
{
	draw_walls();
	if (map[(int)((box()->py + 5) / 45)][(int)((box()->px + pmo()->left_right_ + 5) / 45)] != 1
		&& map[(int)((box()->py - 5) / 45)][(int)((box()->px + pmo()->left_right_ - 5) / 45)] != 1)
		box()->px += pmo()->left_right_;
	if (map[(int)((box()->py + pmo()->up_down_ + 5) / 45)][(int)((box()->px + 5) / 45)] != 1
		&& map[(int)((box()->py + pmo()->up_down_ - 5) / 45)][(int)((box()->px - 5) / 45)] != 1)
	{
		box()->py -= cos(box()->dir_) * pmo()->up_down_;
		box()->px -= sin(box()->dir_) * pmo()->up_down_;
	}
	box()->dir_ += pmo()->diroffset_;
	// draw_lines();
	render_player(floor(box()->py), floor(box()->px), 0xB32134);
	cast_rays();
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
	else if (event_c_ == XK_Left)
		pmo()->diroffset_ = +(*mode_) * 0.02;
	else if (event_c_ == XK_Right)
		pmo()->diroffset_ = -(*mode_) * 0.02;
	return 0;
}

int	main()
{
	double	set_flag;
	double	unset_flag;

	set_flag = 0.03;
	unset_flag = 0;
	box()->dir_ = ( 0 + 90 ) * PI/180;
	pmo()->diroffset_ = 0;
	pmo()->is_moving = true;
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
