/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_init_utils_1.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:48:18 by mel-mouh          #+#    #+#             */
/*   Updated: 2026/01/15 16:50:13 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static t_animation	*animation_walking(t_game *game)
{
	size_t		size;
	static char	*paths[] = {
		"./textures/weapon.xpm",
	};

	size = sizeof(paths) / sizeof(paths[0]);
	return (load_animation_frames(game, paths, size, 1000));
}

static t_animation	*animation_shooting(t_game *game)
{
	size_t		size;
	static char	*paths[] = {
		"./textures/shot/1.xpm",
		"./textures/shot/2.xpm",
		"./textures/shot/3.xpm",
	};

	size = sizeof(paths) / sizeof(paths[0]);
	return (load_animation_frames(game, paths, size, 50));
}

static t_animation	*animation_reload(t_game *game)
{
	size_t		size;
	static char	*paths[] = {
		"./textures/reload/1.xpm",
		"./textures/reload/2.xpm",
		"./textures/reload/3.xpm",
		"./textures/reload/4.xpm",
		"./textures/reload/5.xpm",
		"./textures/reload/6.xpm",
		"./textures/reload/7.xpm",
		"./textures/reload/8.xpm",
		"./textures/reload/9.xpm",
		"./textures/reload/10.xpm",
		"./textures/reload/11.xpm",
		"./textures/reload/12.xpm",
	};

	size = sizeof(paths) / sizeof(paths[0]);
	return (load_animation_frames(game, paths, size, 90));
}

void	set_cardinal_dir(char dir, t_player *player)
{
	if (dir == 'E')
	{
		player->dir = vec2_new(1, 0);
		player->plane = vec2_new(0, 0.66);
	}
	else if (dir == 'W')
	{
		player->dir = vec2_new(-1, 0);
		player->plane = vec2_new(0, -0.66);
	}
	else if (dir == 'N')
	{
		player->dir = vec2_new(0, -1);
		player->plane = vec2_new(0.66, 0);
	}
	else if (dir == 'S')
	{
		player->dir = vec2_new(0, 1);
		player->plane = vec2_new(-0.66, 0);
	}
}

void	player_init_animations(t_game *game)
{
	game->player.state = PLAYER_WALKING;
	game->player.animations[PLAYER_WALKING] = animation_walking(game);
	game->player.animations[PLAYER_SHOOTING] = animation_shooting(game);
	game->player.animations[PLAYER_RELOAD] = animation_reload(game);
}
