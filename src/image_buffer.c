/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_buffer.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:29:54 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/09 19:29:54 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/graphics.h"

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

/**
 * new_img_buff - init new image buffer
 *
 * @game - game object
 * @width: width of the image
 * @height: height of the image
 */
t_data image_new(t_game *game, int width, int height)
{
	t_data img;

	img.img    = mlx_new_image(game->mlx, width, height);
	img.width  = width;
	img.height = height;
	img.addr   = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	return (img);
}

/**
 * image_put_pixel - places a single colored pixel into an image buffer
 *
 * @x: The x-coordinate where the pixel will be placed
 * @y: The y-coordinate where the pixel will be placed
 * @color: The color of the pixel
 *
 * Note: If the (x, y) coordinates are outside the image dimensions or if the
 * buffer is NULL, the function will do nothing
 */
void	image_put_pixel(t_data *buff, int x, int y, unsigned int color)
{
	unsigned int	offset;

	if (!buff || x < 0 || y < 0 || x > buff->width || y > buff->height)
		return ;
	offset = (y * buff->line_len + x * (buff->bpp / 8));
	*((unsigned int *)(buff->addr + offset)) = color;
}


/**
 * image_put_pixel - gives the color at using pixel position
 *
 * @x: The x-coordinate of the pixel
 * @y: The y-coordinate of the pixel
 *
 * Return: the pixel color or UINT_MAX if failed
 */
unsigned int imgae_get_pixel(t_data *img, int x, int y)
{
	int offset;
	unsigned int color;

	if (!img || x < 0 || y < 0 || x > img->width || y > img->height)
		return (UINT_MAX);
	offset= (y * img->line_len + x * (img->bpp / 8));
	color = *((unsigned int *)(img->addr + offset));
	return (color);
}

bool image_load_xpm(t_game *game, t_data *buff, char *path)
{
	t_data img_data;

	printf("Info: Loading the XPM file: '%s'\n", path);
	img_data.img = mlx_xpm_file_to_image(game->mlx, path, &img_data.width, &img_data.height);
	if (!buff)
	{
		printf("Error: %s: Null buffer pointer provided when opening '%s'\n", __func__, path);
		return (false);
	}
	if (!img_data.img)
	{
		printf("Error: '%s': Failed to load XPM file\n", path);
		return (false);
	}
	img_data.addr = mlx_get_data_addr(img_data.img, &img_data.bpp, &img_data.line_len, &img_data.endian);
	*buff = img_data;
	return (true);
}
