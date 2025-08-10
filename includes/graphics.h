/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   graphics.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:23:29 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/09 19:23:29 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GRAPHICS_H
# define GRAPHICS_H

# include "types.h"

/* RGB COLORS */
# define RED 0xFF0000
# define GREEN 0x00FF00
# define BLUE 0x0000FF
# define PURPLE 0xFF00FF

/* Vectors */
t_vec2	vec2_new(double x, double y);
t_vec2	vec2_sub(t_vec2 p1, t_vec2 p2);
double	vec2_len_squared(t_vec2 p1, t_vec2 p2);

/* Common shapes grachics utilities */
void	draw_line(t_data *buff, t_vec2 p1, t_vec2 p2, int color);
void	draw_circle(t_data *buff, t_vec2 center, int radius, int color);
void	draw_filled_circle(void *img, t_vec2 c, int r, int color);
void    draw_square(t_data *image, t_vec2 s, int len, unsigned int color);
void    draw_filled_square(t_data *image, t_vec2 s, int len, unsigned int color);
void	draw_vertical_line(t_data *buff, t_vec2 p1, double len, int color);
void	draw_horizontal_line(t_data *buff, t_vec2 p1, double len, int color);

/* Working with image buffer */
void	image_clear(t_data *img);
t_data	image_new(t_game *game, int width, int height);
void	image_put_pixel(t_data *buff, int x, int y, unsigned int color);

#endif /* GRAPHICS_H */
