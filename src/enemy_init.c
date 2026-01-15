/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:11:22 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 16:11:22 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static t_animation	*enemy_animation_walking(t_game *game)
{
	size_t		size;
	static char	*paths[] = {
		"./textures/enemy/W1.xpm",
		"./textures/enemy/W2.xpm",
		"./textures/enemy/W3.xpm",
		"./textures/enemy/W4.xpm",
		"./textures/enemy/W5.xpm",
		"./textures/enemy/W6.xpm",
		"./textures/enemy/W7.xpm",
		"./textures/enemy/W8.xpm",
	};

	size = sizeof(paths) / sizeof(paths[0]);
	return (load_animation_frames(game, paths, size, 100));
}

static t_animation	*enemy_animation_dead(t_game *game)
{
	size_t		size;
	static char	*paths[] = {
		"./textures/enemy/d.xpm",
	};

	size = sizeof(paths) / sizeof(paths[0]);
	return (load_animation_frames(game, paths, size, 1000));
}

static t_animation	*enemy_animation_attacking(t_game *game)
{
	size_t		size;
	static char	*paths[] = {
		"./textures/enemy/A1.xpm",
		"./textures/enemy/A2.xpm",
		"./textures/enemy/A3.xpm",
		"./textures/enemy/A4.xpm",
		"./textures/enemy/A5.xpm",
	};

	size = sizeof(paths) / sizeof(paths[0]);
	return (load_animation_frames(game, paths, size, 120));
}

void	init_enemies(t_game *game)
{
	t_enemy	*enemy;
	int		i;

	game->enemies = dyn_init_ptr();
	i = 0;
	while (i < 1)
	{
		enemy = ft_calloc(1, sizeof(t_enemy));
		if (!enemy)
		{
			printf("Warn: Faild to allocate memory %s\n", __func__);
			release_game_and_exit(game, EXIT_FAILURE);
		}
		dyn_add_back(game->enemies, enemy);
		enemy->pos = vec2_new(TILE_SIZE * 11 + 40, TILE_SIZE * 1 + 40);
		enemy->animation[ENEMY_WALKING] = enemy_animation_walking(game);
		enemy->animation[ENEMY_ATTACKING] = enemy_animation_attacking(game);
		enemy->animation[ENEMY_DEAD] = enemy_animation_dead(game);
		enemy->state = ENEMY_WALKING;
		enemy->health = ENEMY_MAX_HEALTH;
		enemy->last_attack_time = 0;
		i++;
	}
}
