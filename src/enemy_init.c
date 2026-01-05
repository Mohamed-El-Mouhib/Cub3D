#include "../includes/cub3d.h"

static t_animation *init_enemy_animation(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/enemy_walk/xpm/enemy_frame004.xpm",
		"./textures/enemy_walk/xpm/enemy_frame004.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return (load_animation_frames(game, paths, size));
}

void init_enemies(t_game *game)
{
	t_animation *animation;
	t_enemy *enemy;

	animation = init_enemy_animation(game);
	animation->is_running = true;
	game->enemies = dyn_init_ptr();
	for (int i = 2; i < 5; i++)
	{
		enemy = malloc(sizeof(t_enemy));
		if (!enemy)
			printf("Warn: Faild to allocate memory %s\n", __func__);

		enemy->pos = vec2_new(TILE_SIZE * i, TILE_SIZE * 2);
		enemy->animation = animation;
		dyn_add_back(game->enemies, enemy);
	}
}
