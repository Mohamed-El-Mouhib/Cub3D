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
# define COLOR_NYANZA 0xE5F2C9
# define COLOR_CHAMOIOSE 0x8C705F
# define COLOR_TINY_BLACK 0x1D1E32
# define COLOR_RED  0xFF0000
# define COLOR_GREEN  0x00FF00
# define COLOR_BLUE  0x0000FF
# define COLOR_PURPLE  0xFF00FF
# define COLOR_WHITE  0xFFFFFF
# define COLOR_MINDARO 0xCFF27E
# define COLOR_IGNORE 0x7AD861

/* Vectors */
t_vec2	vec2_new(double x, double y);
t_vec2	vec2_sub(t_vec2 p1, t_vec2 p2);
t_vec2  vec2_add(t_vec2 v1, t_vec2 v2);
t_vec2  vec2_scale(t_vec2 v1, double factor);
t_vec2  vec2_div(t_vec2 v1, double factor);
double	vec2_len_squared(t_vec2 p1, t_vec2 p2);
void    vec2_print(t_vec2 v, char *prefix);

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
bool image_load_xpm(t_game *game, t_data *buff, char *path);

#endif /* GRAPHICS_H */
