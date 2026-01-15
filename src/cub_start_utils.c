/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_start_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 18:02:35 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 18:03:11 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

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
	game_update_time(game);
	game_update_shaking(game);
	player_update_bobbing(game);
	player_update_sway(game);
	player_update_velocity(game);
	player_update_pos(game);
	player_update_state(game);
	player_update_frame(game);
	enemy_update_pos(game, game->enemies->buff[0]);
	enemy_update_state(game, game->enemies->buff[0]);
	enemy_update_frame_all(game);
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
