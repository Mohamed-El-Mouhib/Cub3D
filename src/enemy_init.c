#include "../includes/cub3d.h"

static t_animation *enemy_animation_walking(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/enemy/W1.xpm",
		// "./textures/enemy/W2.xpm",
		// "./textures/enemy/W3.xpm",
		// "./textures/enemy/W4.xpm",
		// "./textures/enemy/W5.xpm",
		// "./textures/enemy/W6.xpm",
		// "./textures/enemy/W7.xpm",
		// "./textures/enemy/W8.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return (load_animation_frames(game, paths, size, 100));
}

static t_animation *enemy_animation_dead(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/enemy/d.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return (load_animation_frames(game, paths, size, 1000));
}

static t_animation *enemy_animation_attacking(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/enemy/A0.xpm",
		// "./textures/enemy/A1.xpm",
		// "./textures/enemy/A2.xpm",
		// "./textures/enemy/A3.xpm",
		// "./textures/enemy/A4.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return (load_animation_frames(game, paths, size, 200));
}

void init_enemies(t_game *game)
{
	t_enemy *enemy;

	game->enemies = dyn_init_ptr();
	for (int i = 2; i < 3; i++)
	{
		enemy = malloc(sizeof(t_enemy));
		if (!enemy)
			printf("Warn: Faild to allocate memory %s\n", __func__);
		ft_bzero(enemy, sizeof(t_enemy));
		enemy->pos = vec2_new(TILE_SIZE * i, TILE_SIZE * 2);
		enemy->animation[ENEMY_WALKING] = enemy_animation_walking(game);
                enemy->animation[ENEMY_ATTACKING] = enemy_animation_attacking(game);
                enemy->animation[ENEMY_DEAD] = enemy_animation_dead(game);
                enemy->state = ENEMY_WALKING;
		enemy->health = ENEMY_MAX_HEALTH;
		enemy->last_attack_time = 0;
		dyn_add_back(game->enemies, enemy);
	}
}
