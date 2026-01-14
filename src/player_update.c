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

char get_map_cell(t_game *game, int x, int y)
{
	if (x >= (int)game->world.map_width || y >= (int)game->world.map_height || x < 0 || y < 0)
		return ('1');
	return (game->world.map[y][x]);
}

/**
 * player_update_pos - Calcualte the player next position using its velocity
 */
#define PLAYER_BUFFER 15.0 

void player_update_pos(t_game *game)
{
	t_vec2  new_pos;
	int     map_x;
	int     map_y;

    new_pos.x = game->player.pos.x + game->player.velocity.x;
    new_pos.y = game->player.pos.y; // Keep Y constant for this check
    if (game->player.velocity.x > 0)
        map_x = (int)((new_pos.x + PLAYER_BUFFER) / TILE_SIZE);
    else
        map_x = (int)((new_pos.x - PLAYER_BUFFER) / TILE_SIZE);

    map_y = (int)(new_pos.y / TILE_SIZE);
    char cell = get_map_cell(game, map_x, map_y);
    if (cell != 'C' && cell != '1')
        game->player.pos.x = new_pos.x;
    new_pos.x = game->player.pos.x; // Use the (potentially updated) X
    new_pos.y = game->player.pos.y + game->player.velocity.y;
    map_x = (int)(new_pos.x / TILE_SIZE);
    if (game->player.velocity.y > 0)
        map_y = (int)((new_pos.y + PLAYER_BUFFER) / TILE_SIZE);
    else
        map_y = (int)((new_pos.y - PLAYER_BUFFER) / TILE_SIZE);
    cell = get_map_cell(game, map_x, map_y);
    if (cell != 'C' && cell != '1')
        game->player.pos.y = new_pos.y;
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
