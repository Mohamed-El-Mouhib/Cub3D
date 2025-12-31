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

	double freq;
	double bob_amount_x;
	double bob_amount_y;

	bob_amount_x = 4 +  game->player.speed_fb / 3;
	bob_amount_y = 2 + game->player.speed_fb / 3;
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

/**
 * player_update_pos_fb - Update player position based on the Forward/Backword
 * movements
 */
void player_update_pos_fb(t_game *game)
{
	t_player *player = &game->player;
	double target_speed;

	target_speed = 0.0;
	if (game->keyboard_events[KEY_W])
		target_speed = PLAYER_MOVE_SPEED * (game->keyboard_events[KEY_SHIFT_L] + 1.0);
	else if (game->keyboard_events[KEY_S])
		target_speed = -PLAYER_MOVE_SPEED * (game->keyboard_events[KEY_SHIFT_L] + 1.0);
	player->speed_fb = lerp(player->speed_fb, target_speed, PLAYER_ACCEL_RATE * game->dt);
	if (fabs(player->speed_fb) < 0.1) 
		player->speed_fb = 0;
	player->pos.x += player->dir.x * player->speed_fb * game->dt;
	player->pos.y += player->dir.y * player->speed_fb * game->dt;
}

/**
 * player_update_pos_lr - Update player position based on the left/right
 * movements
 */
void player_update_pos_lr(t_game *game)
{
	t_player *player = &game->player;
	double target_speed;

	target_speed = 0.0;
	if (game->keyboard_events[KEY_A])
		target_speed = -PLAYER_MOVE_SPEED * (game->keyboard_events[KEY_SHIFT_L] + 1.0);
	else if (game->keyboard_events[KEY_D])
		target_speed = PLAYER_MOVE_SPEED * (game->keyboard_events[KEY_SHIFT_L] + 1.0);
	player->speed_lr = lerp(player->speed_lr, target_speed, PLAYER_ACCEL_RATE * game->dt);
	if (fabs(player->speed_lr) < 0.01) 
		player->speed_lr = 0;
	player->pos.x += player->dir.y * player->speed_lr * game->dt;
	player->pos.y += player->dir.x * player->speed_lr * game->dt;
}
