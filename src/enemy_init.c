#include "../includes/cub3d.h"

static t_animation *enemy_animation_walking(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/enemy_walk/xpm/enemy_frame004.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return (load_animation_frames(game, paths, size));
}

static t_animation *enemy_animation_harmed(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/enemy_walk/xpm/enemy_frame004.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return (load_animation_frames(game, paths, size));
}

static t_animation *enemy_animation_dead(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/enemy_walk/xpm/enemy_frame004.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return (load_animation_frames(game, paths, size));
}

static t_animation *enemy_animation_attacking(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/enemy_walk/xpm/attack1.xpm",
		"./textures/enemy_walk/xpm/attack1.xpm",
		"./textures/enemy_walk/xpm/attack2.xpm",
		"./textures/enemy_walk/xpm/attack2.xpm",
		"./textures/enemy_walk/xpm/attack3.xpm",
		"./textures/enemy_walk/xpm/attack3.xpm",
		"./textures/enemy_walk/xpm/attack4.xpm",
		"./textures/enemy_walk/xpm/attack4.xpm",
		"./textures/enemy_walk/xpm/attack5.xpm",
		"./textures/enemy_walk/xpm/attack5.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return (load_animation_frames(game, paths, size));
}

void init_enemies(t_game *game)
{
	t_animation *animations[ENEMY_STATS_NBR];
	t_enemy *enemy;

	animations[ENEMY_WALKING] = enemy_animation_walking(game);
	animations[ENEMY_HARMED] = enemy_animation_harmed(game);
	animations[ENEMY_DEAD] = enemy_animation_dead(game);
	animations[ENEMY_ATTACKING] = enemy_animation_attacking(game);
	game->enemies = dyn_init_ptr();
	for (int i = 2; i < 3; i++)
	{
		enemy = malloc(sizeof(t_enemy));
		if (!enemy)
			printf("Warn: Faild to allocate memory %s\n", __func__);

		enemy->pos = vec2_new(TILE_SIZE * i, TILE_SIZE * 2);
		enemy->dir = vec2_new(0, 0);
		enemy->animation[ENEMY_WALKING] = animations[ENEMY_WALKING];
		enemy->animation[ENEMY_ATTACKING] = animations[ENEMY_ATTACKING];
		enemy->state = ENEMY_WALKING;
		enemy->health = ENEMY_MAX_HEALTH;
		dyn_add_back(game->enemies, enemy);
	}
}
