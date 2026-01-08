/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_events.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 10:38:40 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 19:03:49 by your_login       ###   ########.fr       */
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
	else if (key_code == XK_space)
		game->keyboard_events[KEY_SPACE] = false;
	else if (key_code == XK_Shift_L)
		game->player.max_speed = PLAYER_WALK_SPEED;
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
	else if (key_code == XK_w)
		game->keyboard_events[KEY_W] = true;
	else if (key_code == XK_s)
		game->keyboard_events[KEY_S] = true;
	else if (key_code == XK_r)
		game->keyboard_events[KEY_R] = true;
	else if (key_code == XK_d)
		game->keyboard_events[KEY_D] = true;
	else if (key_code == XK_a)
		game->keyboard_events[KEY_A] = true;
	else if (key_code == XK_space)
		game->keyboard_events[KEY_SPACE] = true;
	else if (key_code == XK_Shift_L)
		game->player.max_speed = PLAYER_RUN_SPEED;
	return (0);
}


void set_input_dir(t_game *game)
{
	t_vec2 input_dir;

	input_dir.y = game->keyboard_events[KEY_W] - game->keyboard_events[KEY_S];
	input_dir.x = game->keyboard_events[KEY_D] - game->keyboard_events[KEY_A];
	game->player.input_dir = input_dir;
}

void game_handle_keyboard_events(t_game *game)
{
	t_player *player;

	player = &game->player;
	if (game->keyboard_events[KEY_RIGHT])
		player_rotate(game, ROTATE_RIGHT);
	if (game->keyboard_events[KEY_LEFT])
		player_rotate(game, ROTATE_LEFT);
	if (game->keyboard_events[KEY_R] && game->player.state == PLAYER_WALKING)
		game->player.state = PLAYER_RELOAD;
	if (game->keyboard_events[KEY_SPACE] && game->player.state == PLAYER_WALKING)
		game->player.state = PLAYER_SHOOTING;

	// read inputs and set the correct diretion
	set_input_dir(game);

}
