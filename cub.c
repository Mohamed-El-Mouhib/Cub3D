/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:28:28 by aljbari           #+#    #+#             */
/*   Updated: 2025/11/17 22:29:10 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include "includes/graphics.h"
#include "includes/types.h"
#include <stdio.h>

void	game_update_shaking(t_game *game)
{
	game->shake = lerp(game->shake, 0, 0.2);
}

void	game_update_time(t_game *game)
{
	time_t	ct;

	ct = curr_time_ms();
	game->dt = (ct - game->tick) / 1000.0;
	game->tick = curr_time_ms();
}

void	render_message(t_game *game)
{
	t_vec2	head;
	char	c;

	head = vec2_add(game->player.pos, vec2_scale(game->player.dir, TILE_SIZE));
	c = get_map_cell(game, (int)head.x / TILE_SIZE, (int)head.y / TILE_SIZE);
	if (c == 'C')
		image_draw_transparent(game, game->ui[UI_OPEN_DOOR], 360, 450);
	else if (c == 'O')
		image_draw_transparent(game, game->ui[UI_CLOSE_DOOR], 360, 450);
}

void	game_update(t_game *game)
{
	if (game->player.lives == 0)
	{
		printf("R.I.P DARLING MOUSE :(\n");
		release_game_and_exit(game, EXIT_SUCCESS);
	}
	// game
	game_update_time(game);
	game_update_shaking(game);
	// player
	player_update_bobbing(game);
	player_update_sway(game);
	player_update_velocity(game);
	player_update_pos(game);
	player_update_state(game);
	player_update_frame(game);
	// enemy
	enemy_update_pos(game, game->enemies->buff[0]);
	enemy_update_state(game, game->enemies->buff[0]);
	enemy_update_frame_all(game);
	// t_enemy *enemy = game->enemies->buff[0];
	// printf("FRAME Id: %zu\n", enemy->animation[enemy->state]->curr);
}

void	game_render(t_game *game)
{
	raycast_draw_walls(game);
	enemy_draw_all(game);
	ui_render_hearts(game);
	ui_render_ammo(game);
	ui_render_aim(game);
	player_render_frame(game);
	draw_minimap(game);
	mlx_put_image_to_window(game->mlx, game->win, game->scene.img, 0, 0);
}

bool	is_frame_ready(void)
{
	static time_t	last_frame_time;
	static int		frames;

	if (curr_time_ms() - last_frame_time > 4000)
	{
		printf("FRAMES: %d\n", frames / 4);
		last_frame_time = curr_time_ms();
		frames = 0;
	}
	frames++;
	return (true);
}

int	game_loop(t_game *game)
{
	if (!is_frame_ready())
		return (0);
	game_handle_inputs(game);
	game_update(game);
	game_render(game);
	render_message(game);
	return (0);
}

int	destroy(t_game *game)
{
	release_game_and_exit(game, EXIT_SUCCESS);
	return (0);
}

int	main(int ac, char **av)
{
	t_game	game;

	if (ac != 2)
		return (log_error("No map provided"));
	init_game(&game, av[1]);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_hook(game.win, 6, 1L << 6, handle_mouse_move, &game);
		// Our mouse friend
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);         // if key pressed
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);       // if key released
	mlx_hook(game.win, 4, 1L << 2, handle_mouse_press, &game);
	mlx_hook(game.win, 5, 1L << 3, handle_mouse_release, &game);
	mlx_hook(game.win, 17, 0, destroy, &game);
	mlx_loop(game.mlx);
	return (0);
}
