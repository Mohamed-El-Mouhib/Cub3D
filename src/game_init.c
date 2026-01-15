/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:05:39 by aljbari           #+#    #+#             */
/*   Updated: 2025/11/17 22:49:38 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void game_init_ui(t_game *game)
{
	game->ui[UI_BULLET_CONTAINER] =
		assets_get_data_from_path(game, "./textures/Additional/Bullet.xpm");
	game->ui[UI_HEARTH] =
		assets_get_data_from_path(game, "./textures/Additional/Heart.xpm");
	game->ui[UI_DIGITS] =
		assets_get_data_from_path(game, "./textures/Additional/Digits.xpm");
	game->ui[UI_OPEN_DOOR] =
		assets_get_data_from_path(game, "./textures/Additional/Open.xpm");
	game->ui[UI_CLOSE_DOOR] =
		assets_get_data_from_path(game, "./textures/Additional/Close.xpm");
	game->ui[UI_AIM] =
		assets_get_data_from_path(game, "./textures/Additional/CrossHair.xpm");
}

bool init_game_world(char *filename, t_game* game)
{
	ft_bzero(game, sizeof(t_game));
	game->player.pos = vec2_new(-1, -1);
	if (!parse_content(filename, game))
		return (false);
	if (game->player.pos.x == -1 && game->player.pos.y == -1)
		return (false);
	game->mlx = mlx_init();
	if (!game->mlx)
	{
		printf("Failed to allocate mlx\n");
		release_game_and_exit(game, EXIT_FAILURE);
	}
	return (true);
}

void init_game(t_game *game, char *filename)
{
	int i;
	if (!init_game_world(filename, game))
		return;
	game->screen_width = 1280;
	game->screen_height = 720;
	game->assets = dyn_init_ptr();
	i = 0;
	while (i < 4)
	{
		game->wall[i] = *assets_get_data_from_path(game, game->world.values[i]);
		i++;
	}
	game->wall[i] = *assets_get_data_from_path(game, "./textures/Wall/DOOR.xpm");
	game->win = mlx_new_window(game->mlx, game->screen_width, game->screen_height, "MOUSE");
	if (!game->win)
	{
		printf("Failed to allocate window\n");
		release_game_and_exit(game, EXIT_FAILURE);
	}
	game->scene = image_new(game, game->screen_width, game->screen_height);
	game->mouse_pos = vec2_new(0, 0);
	ft_bzero(game->inputs, sizeof(game->inputs));
	player_init(game);
	game->stripes = malloc(sizeof(double) * game->screen_width);
	game->tick = curr_time_ms();
	game->shake = 0;
	game_init_ui(game);
	init_enemies(game);
}
