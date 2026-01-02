#include "../includes/cub3d.h"

static t_animation *animation_idle(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/weapon_animation/xpm/walk_frames_06.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return load_animation_frames(game, paths, size);
}

static t_animation *animation_walking(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/weapon_animation/xpm/walk_frames_06.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return (load_animation_frames(game, paths, size));
}

static t_animation *animation_shooting(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/weapon_animation/xpm/walk_frames_06.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return load_animation_frames(game, paths, size);
}

static t_animation *animation_bored(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/weapon_animation/xpm/walk_frames_06.xpm",
	};

	size = sizeof(paths) / sizeof(paths[0]);
	return load_animation_frames(game, paths, size);
}

void init_player_animations(t_game *game)
{
	game->player.state                       = PLAYER_WALKING;
	game->player.animations[PLAYER_WALKING]  = animation_walking(game);
	game->player.animations[PLAYER_IDLE]     = animation_idle(game);
	game->player.animations[PLAYER_SHOOTING] = animation_shooting(game);
	game->player.animations[PLAYER_BORED]    = animation_bored(game);
}

void player_update_bobing(t_game *game)
{	

	double t;
	double bob_amount_x = 10;
	double bob_amount_y = 20;

	// TODO: Make the bobing speed and brutality related to player speed

	t = game->tick / 166.0;
	game->player.bob.x = sin(t) * bob_amount_x;
	game->player.bob.y = fabs(cos(t)) * -bob_amount_y; // bounce up
}
