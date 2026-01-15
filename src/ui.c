/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:21:22 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 16:21:22 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

#define UI_DIGIT_SIZE 20

void	ui_render_hearts(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->player.lives)
	{
		image_draw_transparent(game, game->ui[UI_HEARTH], 500 + i * 15, 630);
		i++;
	}
}

static void	ui_draw_digit(t_game *game, t_data *image, t_vec2 off, int number)
{
	unsigned int	ignore_color;
	unsigned int	color;
	int				x;
	int				y;
	int				start;

	if (!image)
		return ;
	ignore_color = image_get_pixel(image, 0, 0);
	y = 0;
	while (y < image->height)
	{
		start = UI_DIGIT_SIZE * number;
		x = start;
		while (x < image->width && x < start + UI_DIGIT_SIZE)
		{
			color = image_get_pixel(image, x, y);
			if (color != ignore_color)
				image_put_pixel(&game->scene, x + off.x - start, y + off.y,
					color);
			x++;
		}
		y++;
	}
}

void	ui_draw_number(t_game *game, t_vec2 pos, int number)
{
	int	digits[15];
	int	i;
	int	j;

	i = 0;
	digits[i] = 0;
	while (number > 0)
	{
		digits[i++] = number % 10;
		number /= 10;
	}
	if (i > 0)
		i--;
	j = 0;
	while (i >= 0)
	{
		ui_draw_digit(game, game->ui[UI_DIGITS], vec2_new(pos.x + j
				* UI_DIGIT_SIZE + 2, pos.y), digits[i]);
		j++;
		i--;
	}
}

void	ui_render_ammo(t_game *game)
{
	image_draw_transparent(game, game->ui[UI_BULLET_CONTAINER], 20, 565);
	if (game->player.ammo > 9)
		ui_draw_number(game, vec2_new(65, 643), game->player.ammo);
	else
		ui_draw_number(game, vec2_new(75, 643), game->player.ammo);
}

void	ui_render_aim(t_game *game)
{
	int	x;
	int	y;

	x = 600 + game->player.sway;
	y = 405;
	image_draw_transparent(game, game->ui[UI_AIM], x, y);
}
