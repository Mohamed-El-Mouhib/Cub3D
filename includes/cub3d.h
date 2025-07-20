/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 14:44:05 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/05 17:07:24 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include <fcntl.h>
# include <mlx.h>
# include <X11/Xlib.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>

# define MAP_HEIGHT 10
# define MAP_WIDTH 10

# define KA 97
# define KD 115 
# define KS 100
# define KW 119
# define KE 65307


typedef struct s_parse
{
	int	height;
	int	width;
	int	px;
	int	py;
	void	*win;
	void	*mlx;
}	t_parse;

#endif
