#include "../includes/cub3d.h"

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
		"./textures/enemy_walk/xpm/attack1.xpm",
		"./textures/enemy_walk/xpm/attack2.xpm",
		"./textures/enemy_walk/xpm/attack3.xpm",
		"./textures/enemy_walk/xpm/attack4.xpm",
		"./textures/enemy_walk/xpm/attack5.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return load_animation_frames(game, paths, size);
}

static t_animation *animation_reload(t_game *game)
{
	size_t size;
	static	char *paths[] = {
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
		"./textures/enemy_walk/xpm/enemy_frame005.xpm",
	};

	size = sizeof(paths)/sizeof(paths[0]);
	return load_animation_frames(game, paths, size);
}

void init_player_animations(t_game *game)
{
	game->player.state                       = PLAYER_WALKING;
	game->player.animations[PLAYER_WALKING]  = animation_walking(game);
	game->player.animations[PLAYER_SHOOTING] = animation_shooting(game);
	game->player.animations[PLAYER_RELOAD]   = animation_reload(game);
}

void player_update_bobing(t_game *game)
{	

	double freq;
	double bob_amount_x;
	double bob_amount_y;

	bob_amount_x = 2 + game->player.speed * PLAYER_BOB_AMOUNT;
	bob_amount_y = 4 + game->player.speed * PLAYER_BOB_AMOUNT;
	freq = game->tick / (166.0);
	game->player.bob.x = sin(freq) * bob_amount_x;
	game->player.bob.y = fabs(cos(freq)) * -bob_amount_y; // bounce up
}

void player_update_sway(t_game *game)
{
	double target_sway;
	t_player *player;

	target_sway = 0;
	player = &game->player;
	if (game->keyboard_events[KEY_LEFT])
		target_sway = PLAYER_MAX_SWAY;
	else if (game->keyboard_events[KEY_RIGHT])
		target_sway = -PLAYER_MAX_SWAY;
	if (fabs(player->sway) < 0.1) 
		player->sway = 0;
	player->sway = lerp(player->sway, target_sway, PLAYER_SWAY_SPEED * game->dt);
}
