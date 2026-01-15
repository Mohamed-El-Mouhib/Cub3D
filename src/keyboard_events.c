/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard_events.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 10:38:40 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 17:47:47 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	key_release(int key_code, t_game *game)
{
	if (key_code == XK_Escape)
		release_game_and_exit(game, EXIT_SUCCESS);
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

void	toggle_door(t_game *game)
{
	t_vec2	p;
	t_ivec	player;
	t_ivec	enemy;
	t_ivec	tile;

	p = vec2_add(game->player.pos, vec2_scale(game->player.dir, 90));
	player.x = game->player.pos.x / TILE_SIZE;
	player.y = game->player.pos.y / TILE_SIZE;
	tile.x = p.x / TILE_SIZE;
	tile.y = p.y / TILE_SIZE;
	enemy.x = (*(t_enemy *)game->enemies->buff[0]).pos.x / TILE_SIZE;
	enemy.y = (*(t_enemy *)game->enemies->buff[0]).pos.y / TILE_SIZE;
	if ((tile.x == player.x && tile.y == player.y)
		|| (enemy.x == tile.x && enemy.y == tile.y))
		return ;
	if (get_map_cell(game, tile.x, tile.y) == 'C')
		game->world.map[tile.y][tile.x] = 'O';
	else if (get_map_cell(game, tile.x, tile.y) == 'O')
		game->world.map[tile.y][tile.x] = 'C';
}

int	key_press(int key_code, t_game *game)
{
	if (key_code == XK_Escape)
		release_game_and_exit(game, EXIT_SUCCESS);
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
	else if (key_code == XK_e)
		toggle_door(game);
	else if (key_code == XK_Shift_L)
		game->player.max_speed = PLAYER_RUN_SPEED;
	return (0);
}

void	set_input_dir(t_game *game)
{
	t_vec2	input_dir;

	input_dir.y = game->inputs[KEY_W] - game->inputs[KEY_S];
	input_dir.x = game->inputs[KEY_D] - game->inputs[KEY_A];
	game->player.input_dir = input_dir;
}

void	game_handle_inputs(t_game *game)
{
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
	set_input_dir(game);
}
