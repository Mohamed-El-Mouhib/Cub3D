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
#include "includes/libft.h"

#define TILE_SIZE 50.0
#define TILE_SIZE 50.0
# define WIN_H (TILE_SIZE * 10 + 1)
# define WIN_W (TILE_SIZE * 10 + 1)

// RGB colors
#define RED    0xFF0000
#define GREEN  0x00FF00
#define BLUE   0x0000FF
#define PURPLE 0xFF00FF

#define FRAME_RATE 24

t_vec2 intersect;

void	pixel_put(t_data *buff, int x, int y, unsigned int color)
{
	unsigned int	offset;

	if (x < 0 || y < 0 || x > buff->width || y > buff->height)
		return ;
	offset = (y * buff->line_len + x * (buff->bpp / 8));
	*((unsigned int *)(buff->addr + offset)) = color;
}

typedef struct s_game {
	void	*win;
	void	*mlx;
	t_vec2	player;
	t_vec2	mouse_pos;
	t_vec2	dir;
	t_data scene;
	time_t frame_duration;
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

/**
 * draw_vertical_line - paint vertical line string from p1
 *
 * @buff: buffer image to hold the image data
 * @p1: Starting point of the vertical line
 * @len: length of the line
 *  if len > 0: the line will go down
 *  if len < 0: the line will go up
 * @color: Color of line
 *
 * Return: Northing.
 */
void draw_vertical_line(t_data *buff, t_vec2 p1, double len, int color)
{
	t_vec2 p;

	if (len < 0)
	{
		p1.y += len;
		draw_vertical_line(buff, p1, len * -1, color);
		return ;
	}
	p.x = p1.x;
	p.y = p1.y;
	while (len > 0)
	{
		pixel_put(buff, p.x, p.y, color);
		len--;
		p.y++;
	}
}

/**
 * draw_horizontal_line - paint horizontal line string from p1
 *
 * @buff: buffer image to hold the image data
 * @p1: Starting point of the horizontal line
 * @len: length of the line
 *  if len > 0: the line will fo right
 *  if len < 0: the line will go left
 * @color: Color of line
 *
 * Return: Northing.
 */
void draw_horizontal_line(t_data *buff, t_vec2 p1, double len, int color)
{
	t_vec2 p;

	if (len < 0)
	{
		p1.x += len;
		draw_horizontal_line(buff, p1, len * -1, color);
		return ;
	}
	p.x = p1.x;
	p.y = p1.y;
	while (len > 0)
	{
		pixel_put(buff, p.x, p.y, color);
		len--;
		p.x++;
	}
}

/**
 * draw_line - draw line between two points
 *
 * @p1: the first point
 * @p2: the second point
 * @color: color to use
 *
 * Return: Nothing
 */
void draw_line(t_data *buff, t_vec2 p1, t_vec2 p2, int color)
{
	t_vec2 delta;
	float steps;
	t_vec2 inc;

	delta = vec2_sub(p2, p1);
	steps = fmax(fabs(delta.x), fabs(delta.y));
	inc.x = delta.x / steps;
	inc.y = delta.y / steps;
	int i = 0;
	while (i <= steps)
	{
		pixel_put(buff, round(p1.x), round(p1.y), color);
		p1.x += inc.x;
		p1.y += inc.y;
		i++;
	}
}


void fill_vertical_line(void *img, int cx, int cy, int x, int y, int color)
{
	int i;

	(void) cx;
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


void draw_circle(t_data *buff, t_vec2 center, int radius, int color)
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
		pixel_put(buff, center.x + x, center.y + y, color);
		pixel_put(buff, center.x - x, center.y + y, color);
		pixel_put(buff, center.x + x, center.y - y, color);
		pixel_put(buff, center.x - x, center.y - y, color);
		pixel_put(buff, center.x + y, center.y + x, color);
		pixel_put(buff, center.x + y, center.y - x, color);
		pixel_put(buff, center.x - y, center.y + x, color);
		pixel_put(buff, center.x - y, center.y - x, color);
		x += 1;
	}
}


t_data new_img_buff(t_game *game, int width, int height)
{
	t_data img;

	img.img    = mlx_new_image(game->mlx, width, height);
	img.width  = width;
	img.height = height;
	img.addr   = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	return (img);
}

t_vec2 vec2_new(double x, double y)
{
	t_vec2 vector;

	vector.x = x;
	vector.y = y;
	return (vector);
}

void init_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit_error("Faild to allocate mlx");
	game->win = mlx_new_window(game->mlx, WIN_W, WIN_H, "Hello world!");
	if (!game->win)
		ft_exit_error("Faild to allocate window");
	game->scene = new_img_buff(game, WIN_W, WIN_H);
	game->player = vec2_new(WIN_W / 2, WIN_H / 2);
	game->frame_duration = 1000 / FRAME_RATE;
}

int	key_even_handler(int event_c_)
{
	if (event_c_ == XK_Escape)
		exit(0);
	return (0);
}



void draw_grid(t_data  *buff)
{
	t_vec2 start;
	int i;

	start = vec2_new(0, 0);
	i = 0;
	while (i <= WIN_H)
	{
		start.y = i;
		draw_horizontal_line(buff, start, WIN_W, RED);
		i+= TILE_SIZE;
	}
	start = vec2_new(0, 0);
	i = 0;
	while (i <= WIN_W)
	{
		start.x = i;
		draw_vertical_line(buff, start, WIN_H, RED);
		i+= TILE_SIZE;
	}
}

int handle_mouse_event(int x,int y, t_game *game)
{
	game->mouse_pos.x = x;
	game->mouse_pos.y = y;
	return (0);
}

time_t	curr_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	clear_img(t_data *img)
{
	if (!img->addr) 
		return ;
	ft_bzero(img->addr, img->width * img->height * 4);
}

double (*foo(double dx))(double)
{
	if (dx >= 0)
		return (floor);
	else
		return (ceil);
}

t_vec2 next_x;
t_vec2 next_y;


double vec2_len_squared(t_vec2 p1, t_vec2 p2)
{
	return ((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}
// void vec2_len_square(t_vec2 p1, t_vec2 p2)
// {
// 	return ((p1.x - p2.x) * (p1.x - p2.x))
// }
void	intersection_points(t_vec2 p1, t_vec2 p2)
{
	double k;
	double c;
	t_vec2 sub;
	double x,y;

	sub = vec2_sub(p1, p2);
	if (sub.x == 0)
		k = 0;
	else
		k = sub.y / sub.x;
	c = p1.y - k*p1.x;
	// y = kx + c
	// (y - c) / k= x
	x = foo(sub.x)(p2.x / TILE_SIZE)*TILE_SIZE;
	y = k*x + c;
	next_x = vec2_new(x, y);
	y = foo(sub.y)(p2.y / TILE_SIZE)*TILE_SIZE;
	if (k != 0)
		x = (y - c) / k;
	else
		x = c;
	next_y = vec2_new(x, y);
	double len_x = vec2_len_squared(next_x, p2);
	double len_y = vec2_len_squared(next_y, p2);
	intersect = len_x > len_y ? next_y : next_x;
	return ;
}

int fun(t_game *game)
{
	static time_t last_frame_time;

	if (curr_time_ms() - last_frame_time < game->frame_duration)
		return (0);
	printf("FRAME_RATE: %ld\n", 1000 / (curr_time_ms() - last_frame_time));
	last_frame_time = curr_time_ms();
	clear_img(&game->scene);
	draw_grid(&game->scene);
	draw_circle(&game->scene, game->mouse_pos, 5, RED);
	draw_line(&game->scene, game->player, game->mouse_pos, RED);
	draw_circle(&game->scene, game->player, 5, RED);
	intersection_points(game->player, game->mouse_pos);
	draw_circle(&game->scene, next_x, 5, GREEN);
	draw_circle(&game->scene, next_y, 5, PURPLE);
	draw_circle(&game->scene, intersect, 8, RED);
	// draw_line(&game->scene, next_x, next_y, PURPLE);
	mlx_put_image_to_window(game->mlx, game->win,game->scene.img, 0, 0);
	return (0);
}

int	main()
{
	t_game game;


	init_game(&game);

	mlx_loop_hook(game.mlx, fun, &game);
	mlx_hook(game.win, 02, 1L<<0, key_even_handler, 0);
	mlx_hook(game.win, 03, 1L<<1, key_even_handler, 0);
	mlx_hook(game.win, 06, 1L<<6, handle_mouse_event, &game); // for mouse
	mlx_loop(game.mlx);
	return (0);
}
