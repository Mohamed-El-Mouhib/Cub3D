/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_events.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:02:38 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 11:02:38 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

#define MOUSE_SENSITIVITY 0.002

void player_rotate_mouse(t_game *game, double angle)
{
	t_player *player;
	t_vec2 plane;
	t_vec2 dir;
	double c;
	double s;

	player = &game->player;
	c = cos(angle);
	s = sin(angle);
	dir.x = player->dir.x * c - player->dir.y * s;
	dir.y = player->dir.x * s  + player->dir.y * c;
	plane.x = player->plane.x * c - player->plane.y * s;
	plane.y = player->plane.x * s + player->plane.y * c;
	player->dir = dir;
	player->plane = plane;
}


int handle_mouse_press(int keycode, int x, int y, t_game *game)
{
	(void) x;
	(void) y;

	if (keycode == XK_LEFT_CLICK)
		game->inputs[KEY_LCLICK] = true;
	return (0);
}

int handle_mouse_release(int keycode, int x, int y, t_game *game)
{
	(void) x;
	(void) y;

	if (keycode == XK_LEFT_CLICK)
		game->inputs[KEY_LCLICK] = false;
	return (0);
}

int handle_mouse_move(int x, int y, t_game *game)
{
	int delta_x;

	delta_x = x - game->last_mouse_pos.x;
	if (abs(delta_x) > 200) 
		delta_x = 0;
	if (delta_x != 0)
		player_rotate_mouse(game, delta_x * MOUSE_SENSITIVITY);
	if (x > ((int)game->screen_width - 50))
		mlx_mouse_move(game->mlx, game->win, 60, y);
	if (x < 50)
		mlx_mouse_move(game->mlx, game->win, game->screen_width - 60, y);
	game->last_mouse_pos.x = x;
	game->last_mouse_pos.y = y;
	return (0);
}

