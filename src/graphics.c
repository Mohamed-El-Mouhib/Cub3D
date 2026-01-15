/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:22:30 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:25:46 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * draw_line - draw line between two points
 *
 * @p1: the first point
 * @p2: the second point
 * @color: color to use
 */
void	draw_line(t_data *buff, t_vec2 p1, t_vec2 p2, int color)
{
	t_vec2	delta;
	float	steps;
	t_vec2	inc;
	int		i;

	delta = vec2_sub(p2, p1);
	steps = fmax(fabs(delta.x), fabs(delta.y));
	inc.x = delta.x / steps;
	inc.y = delta.y / steps;
	i = 0;
	while (i <= steps)
	{
		image_put_pixel(buff, round(p1.x), round(p1.y), color);
		p1.x += inc.x;
		p1.y += inc.y;
		i++;
	}
}

/**
 * _fill_vertical_line - used internally to draw circle
 */
static void	_fill_vertical_line(t_data *img, t_vec2 c, t_vec2 p, int color)
{
	int	i;

	if (p.x < 0 || p.x >= img->width || p.y < 0 || p.y >= img->height)
		return ;
	i = c.y;
	while (i != p.y)
	{
		image_put_pixel(img, p.x, i, color);
		if (p.y > c.y)
			i += 1;
		else
			i += -1;
	}
	image_put_pixel(img, p.x, p.y, color);
}

/**
 * draw_filled_circle - Draws a filled circle on an image using the midpoint
 * algorithm
 */
void	draw_filled_circle(void *img, t_vec2 c, int r, int color)
{
	int	x;
	int	y;
	int	p;

	x = 0;
	y = -r;
	p = -r;
	while (x < -y)
	{
		if (p > 0)
			p += 2 * (x + ++y) + 1;
		else
			p += 2 * x + 1;
		_fill_vertical_line(img, c, vec2_new(c.x + x, c.y + y), color);
		_fill_vertical_line(img, c, vec2_new(c.x - x, c.y + y), color);
		_fill_vertical_line(img, c, vec2_new(c.x + x, c.y - y), color);
		_fill_vertical_line(img, c, vec2_new(c.x - x, c.y - y), color);
		_fill_vertical_line(img, c, vec2_new(c.x + y, c.y + x), color);
		_fill_vertical_line(img, c, vec2_new(c.x + y, c.y - x), color);
		_fill_vertical_line(img, c, vec2_new(c.x - y, c.y + x), color);
		_fill_vertical_line(img, c, vec2_new(c.x - y, c.y - x), color);
		x += 1;
	}
}

/**
 * draw_circle - implementation of middle point algorithm to draw circle borders
 *
 * @buff: buffer to put the pixels
 * @c: center
 * @r: raduis
 *
 * Return: Tetsing
 */
void	draw_circle(t_data *buff, t_vec2 c, int r, int color)
{
	int	x;
	int	y;
	int	d;

	x = 0;
	y = r;
	d = 1 - r;
	while (x <= y)
	{
		image_put_pixel(buff, c.x + x, c.y + y, color);
		image_put_pixel(buff, c.x - x, c.y + y, color);
		image_put_pixel(buff, c.x + x, c.y - y, color);
		image_put_pixel(buff, c.x - x, c.y - y, color);
		image_put_pixel(buff, c.x + y, c.y + x, color);
		image_put_pixel(buff, c.x - y, c.y + x, color);
		image_put_pixel(buff, c.x + y, c.y - x, color);
		image_put_pixel(buff, c.x - y, c.y - x, color);
		if (d < 0)
			d += 2 * x + 3;
		else
			d += 2 * (x - y--) + 5;
		x++;
	}
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
void	draw_vertical_line(t_data *buff, t_vec2 p1, double len, int color)
{
	t_vec2	p;

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
		image_put_pixel(buff, p.x, p.y, color);
		len--;
		p.y++;
	}
}
