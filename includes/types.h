/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */ /*                                                    +:+ +:+         +:+     */ /*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 20:11:39 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/09 20:14:07 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include "libft.h"
# include <X11/keysym.h>
# include <fcntl.h>
# include <math.h>
# include <mlx.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

/**
 * t_vec2 - Vectore represeting point at the screen
 *
 * @x: x-coordinate
 * @y: y-coordinate
 */
typedef struct s_vec2
{
	double	x;
	double	y;
}			t_vec2;

typedef struct s_data
{
	void	*img;
	char	*addr;
	int		bpp;
	int		line_len;
	int		endian;
	int		width;
	int		height;
}			t_data;

typedef struct s_parse
{
	int		height;
	int		width;
	double	px;
	double	py;
	void	*win;
	void	*mlx;
	bool	is_moving;
	double	dir_;
	t_data	data;
}			t_parse;

typedef struct s_game
{
	void	*win;
	void	*mlx;
	int	**map;
	size_t map_width;
	size_t map_height;
	size_t screen_width;
	size_t screen_height;
	t_vec2	player;
	t_vec2	mouse_pos;
	t_vec2	dir;
	t_data	scene;
}			t_game;

#endif
