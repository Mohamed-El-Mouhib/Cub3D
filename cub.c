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
#include <mlx.h>
#include <unistd.h>

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

}
void	pixel_put(t_data *buff, int x, int y, unsigned int color)
{
	unsigned int	offset;

	if (x < 0 || y < 0)
		return ;
	offset = (y * buff->line_len + x * (buff->bpp / 8));
	*((unsigned int *)(buff->addr + offset)) = color;
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
typedef struct s_game {
	void	*win;
	void	*mlx;
	t_data scene;
} t_game;

void ft_exit_error(char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));	
	exit(1);
}


int loop()
{
	
	return (0);
}

t_vec2 vec2_sub(t_vec2 p1, t_vec2 p2)
{
	t_vec2 result;

	result.x = p1.x - p2.x;
	result.y = p1.y - p2.y;
	return (result);
}

void draw_vertical_line(t_data *buff, t_vec2 p1, t_vec2 p2, double c)
{
	t_vec2 p;

	p.y = p1.y;
	if (p.y > p2.y)
	{
		while (p.y > p2.y)
		{
			p.x = c;
			pixel_put(buff, p.x, p.y, 0xFF00FF);
			p.y -= 1;
		}
	}
	else
	{
		while (p.y < p2.y)
		{
			p.x = c;
			pixel_put(buff, p.x, p.y, 0xFF00FF);
			p.y += 1;
		}
	}
}

void draw_line(t_data *buff, t_vec2 p1, t_vec2 p2)
{
	double k;
	double c;
	t_vec2 p;
	t_vec2 sub;

	sub = vec2_sub(p1, p2);
	if (sub.x == 0)
		k = 0;
	else
		k = sub.y / sub.x;
	c = p1.y - k*p1.x;
	if (k == 0)
		draw_vertical_line(buff, p1, p2, c);
	else
	{
		p.x = p1.x;
		if (p.x > p2.x)
		{
			while (p.x > p2.x)
			{
				p.y = p.x *k + c;
				pixel_put(buff, p.x, p.y, 0xFF00FF);
				p.x -= 1;
			}
		}
		else
		{
			while (p.x < p2.x)
			{
				p.y = p.x *k + c;
				pixel_put(buff, p.x, p.y, 0xFF00FF);
				p.x += 1;
			}
		}
	}
}


void fill_vertical_line(void *img, int cx, int cy, int x, int y, int color)
{
	int i;

	if (x < 0 || x >= WIN_W || y < 0 || y >= WIN_H)
		return;
	i = cy;
	while (i != y)
	{
		pixel_put(img, x, i, color);
		i += (y > cy ? 1 : -1);
	}
	pixel_put(img, x, y, color);
}

void draw_filled_circle(void *img, t_vec2 c, int r, int color)
{
	int x = 0;
	int y = -r;
	int p = -r;

	x = 0;
	y = -r;
	p = -r;
	while (x < -y)
	{
		if (p > 0)
		{
			y += 1;
			p += 2 * (x + y) + 1;
		}
		else
	{
			p += 2 * x + 1;
		}
		fill_vertical_line(img, c.x, c.y, c.x + x, c.y + y, color);
		fill_vertical_line(img, c.x, c.y, c.x - x, c.y + y, color);
		fill_vertical_line(img, c.x, c.y, c.x + x, c.y - y, color);
		fill_vertical_line(img, c.x, c.y, c.x - x, c.y - y, color);
		fill_vertical_line(img, c.x, c.y, c.x + y, c.y + x, color);
		fill_vertical_line(img, c.x, c.y, c.x + y, c.y - x, color);
		fill_vertical_line(img, c.x, c.y, c.x - y, c.y + x, color);
		fill_vertical_line(img, c.x, c.y, c.x - y, c.y - x, color);
		x += 1;
	}
}


void draw_circle(t_data *buff, t_vec2 center, int radius)
{
	int x = 0;
	int y = -radius;
	int p = -radius;


	while (x < -y)
	{
		if (p > 0)
		{
			y += 1;
			p += 2 * (x + y) + 1;
		}
		else
			p += 2 * x + 1;
		pixel_put(buff, center.x + x, center.y + y, 0xFF00FF);
		pixel_put(buff, center.x - x, center.y + y, 0xFF00FF);
		pixel_put(buff, center.x + x, center.y - y, 0xFF00FF);
		pixel_put(buff, center.x - x, center.y - y, 0xFF00FF);
		pixel_put(buff, center.x + y, center.y + x, 0xFF00FF);
		pixel_put(buff, center.x + y, center.y - x, 0xFF00FF);
		pixel_put(buff, center.x - y, center.y + x, 0xFF00FF);
		pixel_put(buff, center.x - y, center.y - x, 0xFF00FF);
		x += 1;
	}
}

void init_game(t_game *game)
{
	t_data	img;

	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit_error("Faild to allocate mlx");
	game->win = mlx_new_window(game->mlx, WIN_W, WIN_H, "Hello world!");
	if (!game->win)
		ft_exit_error("Faild to allocate window");
	img.img = mlx_new_image(game->mlx, WIN_W, WIN_H);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	game->scene = img;
}


t_data new_img_buff(t_game *game, int width, int height)
{
	t_data img;

	img.img = mlx_new_image(game->mlx, width, height);
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	return (img);
}

t_vec2 vec2_new(double x, double y)
{
	t_vec2 vector;

	vector.x = x;
	vector.y = y;
	return (vector);
}

int	main()
{
	t_game game;

	init_game(&game);
	draw_filled_circle(&game.scene, vec2_new(0, 0), 200, 0xFF00FF);
	draw_circle(&game.scene, vec2_new(100, 100), 200);
	draw_circle(&game.scene, vec2_new(100, 200), 200);
	draw_line(&game.scene, vec2_new(100, 100), vec2_new(50, 200));
	mlx_put_image_to_window(game.mlx, game.win,game.scene.img, 0, 0);
	mlx_loop(game.mlx);

	return (0);
}
