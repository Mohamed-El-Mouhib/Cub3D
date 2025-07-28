/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:44:05 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/28 20:41:57 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include <fcntl.h>
# include <mlx.h>
# include <math.h>
# include <X11/keysym.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define MAP_HEIGHT 6
# define MAP_WIDTH 6
# define MAX_DISTANCE 50
# define FOV 60
# define PI 3.14159
# define TILE_S 60
typedef struct	s_data
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
}	t_data;

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef struct s_parse
{
	int	height;
	int	width;
	double	px;
	double	py;
	void	*win;
	void	*mlx;
	bool	is_moving;
	double	dir_;
	t_data	data;
}	t_parse;

typedef struct	s_cal
{
	double	y_iter;
	double	x_iter;
}	t_cal;

typedef struct s_movment
{
	double	left_right_;
	double	up_down_;
	double	diroffset_;
	bool	is_moving;
}	t_movment;


void	my_pixel_put(int x, int y, unsigned int color);

#endif
