#include "../includes/cub3d.h"

/**
 * player_update_velocity - Calculate the player velocity using from inputs
 *
 * player->input_dir vector will be calculated from the inputs handler
 * Veclociy: representing the player motion, combining its speed and its direction
 */
void player_update_velocity(t_game *game)
{
	t_vec2 forward;
	t_vec2 right;
	t_player *p;
	t_vec2 target_vel;

	p = &game->player;
	right = vec2_new(-p->dir.y, p->dir.x);
	forward = p->dir;
	right =  vec2_scale(right, p->input_dir.x);
	forward = vec2_scale(forward, p->input_dir.y);
	target_vel = vec2_scale(vec2_add(forward, right), p->max_speed * game->dt);
	p->velocity = vec2_lerp(p->velocity, target_vel, PLAYER_ACCEL_RATE);
	if (fabs(p->velocity.x) < 0.001)
		p->velocity.x = 0;
	if (fabs(p->velocity.y) < 0.001)
		p->velocity.y = 0;
	p->speed = hypot(p->velocity.x, p->velocity.y) * 100;
}

/**
 * player_update_pos - Calcualte the player next position using its velocity
 */
void player_update_pos(t_game *game)
{
	game->player.pos = vec2_add(game->player.pos, game->player.velocity);
}

/**
 * player_update_bobing - Calcualte current player bob amount
 *
 * Bobbing is the animation of the player shaking..
 * The amount is related to speed, resulting in more shaking at higher speeds.
 */
void player_update_bobbing(t_game *game)
{	

	double freq;
	double bob_amount_x;
	double bob_amount_y;

	if (game->dt > 0.1)
		return ;
	bob_amount_x = 2 + game->player.speed * PLAYER_BOB_AMOUNT;
	bob_amount_y = 4 + game->player.speed * PLAYER_BOB_AMOUNT;
	freq = game->tick / (166.0);
	game->player.bob.x = sin(freq) * bob_amount_x;
	game->player.bob.y = fabs(cos(freq)) * -bob_amount_y; // bounce up
}

/**
 * player_update_sway - Calcuate the sway amount
 *
 * Sway is the animation that results when the player rotates.
 * The physics behind it are very simple: player will appear to go outside the
 * screen while rotating
 */
void player_update_sway(t_game *game)
{
	double target_sway;
	t_player *player;

	if (game->dt > 0.1)
		return ;
	target_sway = 0;
	player = &game->player;
	if (game->inputs[KEY_LEFT])
		target_sway = PLAYER_MAX_SWAY;
	else if (game->inputs[KEY_RIGHT])
		target_sway = -PLAYER_MAX_SWAY;
	if (fabs(player->sway) < 0.1) 
		player->sway = 0;
	player->sway = lerp(player->sway, target_sway, PLAYER_SWAY_SPEED * game->dt);
}

/**
 * player_update_frame - update the current player frame
 */
void player_update_frame(t_game *game)
{
	animation_cycle(game, game->player.animations[game->player.state]);
}
