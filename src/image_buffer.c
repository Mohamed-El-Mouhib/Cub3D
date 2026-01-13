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
unsigned int image_get_pixel(t_data *img, int x, int y)
{
	int offset;
	unsigned int color;

	if (!img || x < 0 || y < 0 || x > img->width || y > img->height)
		return (UINT_MAX);
	offset= (y * img->line_len + x * (img->bpp / 8));
	color = *((unsigned int *)(img->addr + offset));
	return (color);
}

/**
 * image_load_xpm - load xpm file from the @path into the @buff
 */
bool image_load_xpm(t_game *game, t_data *buff, char *path)
{
	t_data img;

	printf("Info: loading the XPM file: '%s'\n", path);
	if (!buff)
	{
		printf("Error: null buffer when opening: '%s'\n", path);
		exit(1);
		return (false);
	}
	img.img = mlx_xpm_file_to_image(game->mlx, path, &img.width, &img.height);
	if (!img.img)
	{
		printf("Error: failed to load XPM file: '%s'\n", path);
		exit(1);
		return (false);
	}
	img.addr = mlx_get_data_addr(img.img, &img.bpp, &img.line_len, &img.endian);
	*buff = img;
	return (true);
}

/**
 * image_draw_transparent - draw the @image at (x_off, y_off) position of the
 * screen, the color at 0,0 of the @image will be ignored, this is a dirty trick
 * to work around the library's lack of ability to rander PNG files
 */
void image_draw_transparent(t_game *game, t_data *image, double x_off, double y_off)
{
	unsigned int ignore_color; 
	unsigned int color; 
	int x;
	int y;

	if (!image)
		return ;
	ignore_color = image_get_pixel(image, 0, 0);
	y = 0;
	while (y < image->height)
	{
		x = 0;
		while (x < image->width)
		{
			color = image_get_pixel(image, x, y);
			if (color != ignore_color)
				image_put_pixel(&game->scene, x + x_off, y + y_off, color);
			x++;
		}
		y++;
	}
}
