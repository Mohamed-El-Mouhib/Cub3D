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
#include <mlx.h>

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

#define MOUSE_SENSITIVITY 0.003

int handle_mouse_event(int x,int y, t_game *game)
{
	int delta_x;
	// game->mouse_pos.x = x;
	// game->mouse_pos.y = y;
	delta_x = x - game->last_mouse_pos.x;
	if (abs(delta_x) > 200) 
		delta_x = 0;
	// printf("%f\n", delta_x * MOUSE_SENSITIVITY);
	// printf("Delta %d\n", delta_x);
	if (delta_x != 0)
	{
		player_rotate_mouse(game, delta_x * MOUSE_SENSITIVITY);
	}
	// mlx_mouse_move(game->mlx, game->win,game->screen_width / 2.0, game->screen_height / 2.0);
	if (x > ((int)game->screen_width - 50))
		mlx_mouse_move(game->mlx, game->win, 60, y);
	if (x < 50)
		mlx_mouse_move(game->mlx, game->win, game->screen_width - 60, y);
	game->last_mouse_pos.x = x;
	game->last_mouse_pos.y = y;
	return (0);
}

