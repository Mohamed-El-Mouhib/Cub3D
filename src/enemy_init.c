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

void	enemy_init_pos(t_game *game, t_enemy *enemy)
{
	t_vec2	p_pos;
	int		dist;

	p_pos = vec2_new(game->player.pos.x / TILE_SIZE, game->player.pos.y
			/ TILE_SIZE);
	dist = 0;
	while (dist < 5)
	{
		if (get_map_cell(game, p_pos.x + 1, p_pos.y) == '0')
			p_pos.x++;
		else if (get_map_cell(game, p_pos.x, p_pos.y + 1) == '0')
			p_pos.y++;
		else if (get_map_cell(game, p_pos.x, p_pos.y - 1) == '0')
			p_pos.y--;
		else if (get_map_cell(game, p_pos.x - 1, p_pos.y) == '0')
			p_pos.x--;
		dist++;
	}
	enemy->pos.x = p_pos.x * TILE_SIZE;
	enemy->pos.y = p_pos.y * TILE_SIZE;
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
		enemy_init_pos(game, enemy);
		enemy->animation[ENEMY_WALKING] = enemy_animation_walking(game);
		enemy->animation[ENEMY_ATTACKING] = enemy_animation_attacking(game);
		enemy->animation[ENEMY_DEAD] = enemy_animation_dead(game);
		enemy->state = ENEMY_WALKING;
		enemy->health = ENEMY_MAX_HEALTH;
		enemy->last_attack_time = curr_time_ms();
		i++;
	}
}
