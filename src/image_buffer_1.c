/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer_1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:26:10 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:26:24 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

/**
 * clear_img - clear img buffer by makeing all pixel zero
 *
 * @img: image to be reseted
 */
void	image_clear(t_data *img)
{
	if (!img->addr)
		return ;
	ft_bzero(img->addr, img->width * img->height * 4);
}
