/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics_1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:24:10 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:24:17 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
void	draw_horizontal_line(t_data *buff, t_vec2 p1, double len, int color)
{
	t_vec2	p;

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
		image_put_pixel(buff, p.x, p.y, color);
		len--;
		p.x++;
	}
}

/**
 * draw_filled_square - Draws a filled square on the image buffer
 *
 * @s: top left corner coordinations of the square
 * @len: side length of square
 */
void	draw_filled_square(t_data *image, t_vec2 s, int len, unsigned int color)
{
	t_vec2	p;
	int		max_x;

	p = s;
	max_x = p.x + len;
	while (p.x < max_x)
	{
		draw_vertical_line(image, p, len, color);
		p.x++;
	}
}

/**
 * draw_square - Draws borders of square on image buffer
 *
 * @s: top left corner coordinations of the square
 * @len: side length of square
 */
void	draw_square(t_data *image, t_vec2 s, int len, unsigned int color)
{
	draw_vertical_line(image, s, len, color);
	draw_horizontal_line(image, s, len, color);
	s.x += len;
	draw_vertical_line(image, s, len, color);
	s.y += len;
	draw_horizontal_line(image, s, -len, color);
}
