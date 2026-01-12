#include "../includes/cub3d.h"

void release_player_animations(t_game *game)
{
	int i;

	i = 0;
	while (i < PLAYER_STATS_NBR)
		free(game->player.animations[i++]);
}

void realse_player(t_game *game)
{
	release_player_animations(game);
}

void release_enemy(void *e)
{
	int i;
	t_enemy *enemy;

	enemy = e;
	i = 0;
	while (i < ENEMY_STATS_NBR)
		free(enemy->animation[i++]);
	free(enemy);
}

void realse_enemies(t_game *game)
{
	dyn_erase(game->enemies, release_enemy);
	free(game->enemies);
}

void release_asset(t_game *game, t_data *data)
{
	mlx_destroy_image(game->mlx, data->img);
	free(data);
}

void release_assets(t_game *game)
{
	size_t i;

	i = 0;
	while (i < game->assets->length)
		release_asset(game, game->assets->buff[i++]);
	free(game->assets->buff);
	free(game->assets);
	game->enemies = NULL;
}

void release_and_exit(t_game *game)
{
	free(game->stripes);
	realse_player(game);
	realse_enemies(game);
	release_assets(game);
	mlx_destroy_window(game->mlx, game->win);
	mlx_destroy_display(game->mlx);
	free(game->mlx);
	exit(0);
}
