/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 21:54:38 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/29 19:51:51 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

t_parse	*box(void)
{
	static t_parse var;

	return	(&var);
}

t_cal	*iterators(void)
{
	static t_cal	var;

	return  (&var);
}

t_movment	*pmo(void)
{
	static t_movment pp;

	return (&pp);
}

t_ray	arr[105];

int map[MAP_HEIGHT][MAP_WIDTH] = {
  {1,1,1,1,1,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,0,0,0,0,1},
  {1,0,0,0,6,1},
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
				box()->py = (double)i * TILE_S + 30;
				box()->px = (double)j * TILE_S + 30;
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
	while (++i < TILE_S - 2)
	{
		j = -1;
		while (++j < TILE_S - 2)
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
	while (++i < 9)
	{
		j = -1;
		while (++j < 9)
		{
			if ((!i || i == 8 || !j || j == 8) && (y - 5 + j + 1) % TILE_S != 0 && (x - 5 + i + 1) % TILE_S != 0
				&& (y - 5 + j + 2) % TILE_S != 0 && (x - 5 + i + 2) % TILE_S != 0)
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
				render_wall(j * TILE_S, i * TILE_S, 0x555555);
			else
				render_wall(j * TILE_S, i * TILE_S, 0x808080);
		}
	}
}

void	draw_rays(void)
{
	double	i;
	int	j;
	double	y;
	double	x;

	int c=0;
	i = box()->dir_ - (PI / 6);
	while (i < box()->dir_ + (PI / 6))
	{
		j = 0;
		while (1)
		{
			y = box()->py + cos(i) * j;
			x = box()->px + sin(i) * j;
			if (map[(int)y / TILE_S][(int)x / TILE_S] == 1)
				break ;
			if (((int)y + 1) % TILE_S != 0 && ((int)x + 1) % TILE_S != 0
				&& ((int)y + 2) % TILE_S != 0 && ((int)x + 2) % TILE_S != 0)
				my_pixel_put((int)x, (int)y, 0xFFFFFF);
			j++;
		}
		c++;
		i += 0.01;
	}
}

void	cast_rays(void)
{
	double	i;
	int	j;
	int	it;
	double	y;
	double	x;

	i = box()->dir_ - (PI / 6);
	it = 0;
	while (i < box()->dir_ + (PI / 6))
	{
		j = 0;
		while (1)
		{
			y = box()->py + cos(i) * j;
			x = box()->px + sin(i) * j;
			if (map[(int)y / TILE_S][(int)x / TILE_S] == 1)
			{
				arr[it].rx = x;
				arr[it].ry = y;
				it++;
				break ;
			}
			j++;
		}
		i += 0.01;
	}
}

void	draw_floor_ceiling(void)
{
	int	x;
	int	y;

	y = 0;
	while (y < WIN_H)
	{
		x = 0;
		while (x < WIN_W)
		{
			if (y < WIN_H/2)
				my_pixel_put(x, y, 0x808080);
			else
				my_pixel_put(x, y, 0xffe4c4);
			x++;
		}
		y++;
	}
}

void	draw_row_wall(int dist, int col)
{
	int	j;

	j = 0;
	while (j < dist/2)
	{
		my_pixel_put(col, WIN_H/2 - j, 0xD10096);
		my_pixel_put(col+ 1, WIN_H/2 - j, 0xD10096);
		my_pixel_put(col+ 2, WIN_H/2 - j, 0xD10096);
		my_pixel_put(col+ 3, WIN_H/2 - j, 0xD10096);
		my_pixel_put(col+ 4, WIN_H/2 - j, 0xD10096);
		my_pixel_put(col, WIN_H/2 + j, 0xD10096);
		my_pixel_put(col+ 1, WIN_H/2 + j, 0xD10096);
		my_pixel_put(col+ 2, WIN_H/2 + j, 0xD10096);
		my_pixel_put(col+ 3, WIN_H/2 + j, 0xD10096);
		my_pixel_put(col+ 4, WIN_H/2 + j, 0xD10096);
		j++;
	}
}

void	construct_wall(void)
{
	double	dist;
	int	i;

	i = 0;
	while (i < 105)
	{
		dist = (WIN_H * TILE_S) / hypot(box()->px - arr[i].rx, box()->py - arr[i].ry);
		if ((int)dist > WIN_H)
			dist = (int)WIN_H;
		draw_row_wall(dist, i*5);
		i++;
	}
}

int	function_handle_(void)
{
	draw_floor_ceiling();
	cast_rays();
	construct_wall();
	// printf("here\n");
	// draw_walls();
	// if (map[(int)((box()->py + 5) / TILE_S)][(int)((box()->px + pmo()->left_right_ + 5) / TILE_S)] != 1
	// 	&& map[(int)((box()->py - 5) / TILE_S)][(int)((box()->px + pmo()->left_right_ - 5) / TILE_S)] != 1)
		box()->px += pmo()->left_right_;
	// if (map[(int)((box()->py + pmo()->up_down_ + 5) / TILE_S)][(int)((box()->px + 5) / TILE_S)] != 1
	// 	&& map[(int)((box()->py + pmo()->up_down_ - 5) / TILE_S)][(int)((box()->px - 5) / TILE_S)] != 1)
	// {
		box()->py -= cos(box()->dir_) * pmo()->up_down_;
		box()->px -= sin(box()->dir_) * pmo()->up_down_;
	// }
	box()->dir_ += pmo()->diroffset_;
	// render_player(floor(box()->py), floor(box()->px), 0xB32134);
	// draw_rays();
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
		pmo()->diroffset_ = -(*mode_) * 0.02;
	else if (event_c_ == XK_Right)
		pmo()->diroffset_ = +(*mode_) * 0.02;
	return 0;
}

int	main()
{
	double	set_flag;
	double	unset_flag;

	set_flag = 0.08;
	unset_flag = 0;
	box()->dir_ = (30 + 90) * PI/180;
	pmo()->diroffset_ = 0;
	pmo()->is_moving = true;
	init();
	box()->mlx = mlx_init();
	// box()->win = mlx_new_window(box()->mlx, box()->height*TILE_S, box()->width*TILE_S, "Cub3d test");
	// box()->data.img = mlx_new_image(box()->mlx, box()->height * TILE_S, box()->width * TILE_S);
	box()->win = mlx_new_window(box()->mlx, WIN_W, WIN_H, "Cub3d test");
	box()->data.img = mlx_new_image(box()->mlx, WIN_W, WIN_H);
	box()->data.addr = mlx_get_data_addr(box()->data.img, &box()->data.bpp, &box()->data.line_len, &box()->data.endian);
	mlx_hook(box()->win, 02, 1L<<0, key_set_, &set_flag);
	mlx_hook(box()->win, 03, 1L<<1, key_set_, &unset_flag);
	mlx_loop_hook(box()->mlx, function_handle_, NULL);
	mlx_loop(box()->mlx);
	return (0);
}
