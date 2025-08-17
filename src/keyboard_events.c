/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_events.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 10:38:40 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 10:38:40 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/cub3d.h"

int	key_release(int key_code, t_game *game)
{
	if (key_code == XK_Escape)
		exit(0);
	else if (key_code == XK_Right)
		game->keyboard_events[KEY_RIGHT] = false;
	else if (key_code == XK_Left)
		game->keyboard_events[KEY_LEFT] = false;
	else if (key_code == XK_w)
		game->keyboard_events[KEY_W] = false;
	else if (key_code == XK_s)
		game->keyboard_events[KEY_S] = false;
	else if (key_code == XK_r)
		game->keyboard_events[KEY_R] = false;
	else if (key_code == XK_d)
		game->keyboard_events[KEY_D] = false;
	else if (key_code == XK_a)
		game->keyboard_events[KEY_A] = false;
	return (0);
}

int	key_press(int key_code, t_game *game)
{
	if (key_code == XK_Escape)
		exit(0);
	else if (key_code == XK_Right)
		game->keyboard_events[KEY_RIGHT] = true;
	else if (key_code == XK_Left)
		game->keyboard_events[KEY_LEFT] = true;
	else if (key_code == 'w')
		game->keyboard_events[KEY_W] = true;
	else if (key_code == 's')
		game->keyboard_events[KEY_S] = true;
	else if (key_code == 'r')
		game->keyboard_events[KEY_R] = true;
	else if (key_code == 'd')
		game->keyboard_events[KEY_D] = true;
	else if (key_code == 'a')
		game->keyboard_events[KEY_A] = true;
	return (0);
}

void game_handle_keyboard_events(t_game *game)
{
	t_player *player;

	player = &game->player;
	if (game->keyboard_events[KEY_RIGHT])
		player_rotate(player, ROTATE_RIGHT);
	if (game->keyboard_events[KEY_LEFT])
		player_rotate(player, ROTATE_LEFT);
	if (game->keyboard_events[KEY_W])
		player_move(player, 2, MOVE_FORWARD);
	if (game->keyboard_events[KEY_S])
		player_move(player, 2, MOVE_BACKWARD);
	if (game->keyboard_events[KEY_R])
		game->player.pos = vec2_new(TILE_SIZE * 4, TILE_SIZE * 4);
	if (game->keyboard_events[KEY_D])
		player_move(player, 2, MOVE_RIGHT);
	if (game->keyboard_events[KEY_A])
		player_move(player, 2, MOVE_LEFT);
}
