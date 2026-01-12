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
		game->inputs[KEY_RIGHT] = false;
	else if (key_code == XK_Left)
		game->inputs[KEY_LEFT] = false;
	else if (key_code == XK_w)
		game->inputs[KEY_W] = false;
	else if (key_code == XK_s)
		game->inputs[KEY_S] = false;
	else if (key_code == XK_r)
		game->inputs[KEY_R] = false;
	else if (key_code == XK_d)
		game->inputs[KEY_D] = false;
	else if (key_code == XK_a)
		game->inputs[KEY_A] = false;
	else if (key_code == XK_space)
		game->inputs[KEY_SPACE] = false;
	else if (key_code == XK_Shift_L)
		game->player.max_speed = PLAYER_WALK_SPEED;
	return (0);
}


void	toggle_door(t_game* game)
{
	t_vec2 p;
	int p_x = (int)(game->player.pos.x / TILE_SIZE);
	int p_y = (int)(game->player.pos.y / TILE_SIZE);
	int t_x = (int)(p.x / TILE_SIZE);
	int t_y = (int)(p.y / TILE_SIZE);

	p = vec2_add(game->player.pos, vec2_scale(game->player.dir, TILE_SIZE));
	p_x = game->player.pos.x / TILE_SIZE;
	p_y = game->player.pos.y / TILE_SIZE;
	t_x = p.x / TILE_SIZE;
	t_y = p.y / TILE_SIZE;
	if (t_x == p_x && t_y == p_y)
		return;
	if (game->world.map[t_y][t_x] == 'C')
	{
		game->world.map[t_y][t_x] = 'O';
	}
	else if (game->world.map[t_y][t_x] == 'O')
	{
		game->world.map[t_y][t_x] = 'C';
	}
}

int	key_press(int key_code, t_game *game)
{
	if (key_code == XK_Escape)
		game->inputs[KEY_ESCAPE] = true;
	else if (key_code == XK_Right)
		game->inputs[KEY_RIGHT] = true;
	else if (key_code == XK_Left)
		game->inputs[KEY_LEFT] = true;
	else if (key_code == XK_w)
		game->inputs[KEY_W] = true;
	else if (key_code == XK_s)
		game->inputs[KEY_S] = true;
	else if (key_code == XK_r)
		game->inputs[KEY_R] = true;
	else if (key_code == XK_d)
		game->inputs[KEY_D] = true;
	else if (key_code == XK_a)
		game->inputs[KEY_A] = true;
	else if (key_code == XK_space)
		game->inputs[KEY_SPACE] = true;
	else if (key_code == XK_Shift_L)
		game->player.max_speed = PLAYER_RUN_SPEED;
	return (0);
}


void set_input_dir(t_game *game)
{
	t_vec2 input_dir;

	input_dir.y = game->inputs[KEY_W] - game->inputs[KEY_S];
	input_dir.x = game->inputs[KEY_D] - game->inputs[KEY_A];
	game->player.input_dir = input_dir;
}

void game_handle_inputs(t_game *game)
{
	if (game->inputs[KEY_ESCAPE])
		release_and_exit(game);
	if (game->inputs[KEY_RIGHT])
		player_rotate(game, ROTATE_RIGHT);
	if (game->inputs[KEY_LEFT])
		player_rotate(game, ROTATE_LEFT);
	if (game->inputs[KEY_R] && game->player.state == PLAYER_WALKING)
		game->player.state = PLAYER_RELOAD;
	if (game->inputs[KEY_SPACE] && game->player.state == PLAYER_WALKING)
		game->player.state = PLAYER_SHOOTING;
	if (game->inputs[KEY_LCLICK] && game->player.state == PLAYER_WALKING)
		game->player.state = PLAYER_SHOOTING;

	// read inputs and set the correct desired diretion
	set_input_dir(game);

}
