#include "../includes/cub3d.h"


/**
 * has_los - chack if point @from has line of sight into point @to
 */
int has_los(t_game *g, t_vec2 from, t_vec2 to)
{
	t_vec2  d;
	t_vec2  curr;
	float   dist;
	int     steps;
	char	c;

	d = vec2_sub(to, from);
	dist = sqrt(vec2_len_squared(to, from));
	// if (dist > 300) return (0);

	d = vec2_scale(d, (TILE_SIZE / 2.0) / dist); 
	steps = (int)(dist / (TILE_SIZE / 2.0));
	curr = from;

	while (steps-- > 0)
	{
		curr = vec2_add(curr, d);
		c = get_map_cell(g, (int)curr.x / TILE_SIZE, (int)curr.y / TILE_SIZE);
		if (c == '1' || c == 'C')
			return (0);
	}
	return (1);
}


/**
 * Checks collision with a radius buffer to prevent clipping
 */
int can_move(t_game *g, double x, double y)
{
	int	pad;
	char	cell;

	pad = TILE_SIZE / 4;
	cell = get_map_cell(g, (x + pad) / TILE_SIZE, (y + pad) / TILE_SIZE);
	if (cell == '1' || cell == 'C')
		return 0;
	cell = get_map_cell(g, (x - pad) / TILE_SIZE, (y - pad) / TILE_SIZE);
	if (cell == '1' || cell == 'C')
		return 0;
	cell = get_map_cell(g, (x - pad) / TILE_SIZE, (y + pad) / TILE_SIZE);
	if (cell == '1' || cell == 'C')
		return 0;
	cell = get_map_cell(g, (x + pad) / TILE_SIZE, (y - pad) / TILE_SIZE);
	if (cell == '1' || cell == 'C')
		return 0;
	return (1);
}

bool enemy_is_too_close(t_game *game, t_enemy *enemy)
{
	double   dist_sq;

	dist_sq = vec2_len_squared(enemy->pos, game->player.pos);
	if (dist_sq < TILE_SIZE * TILE_SIZE) 
	{
		if (game->tick - enemy->last_attack_time > 4000)
		{
			enemy->state = ENEMY_ATTACKING;
			enemy->last_attack_time = game->tick;
		}
		return (true);
	}
	return (false);
}

void enemy_update_pos(t_game *game, t_enemy *enemy)
{
	t_vec2  dir;
	t_vec2  next;

	if (enemy->state == ENEMY_DEAD)
		return ;
	enemy->moving = false;
	if (enemy_is_too_close(game, enemy)) 
		return;
	if (!has_los(game, enemy->pos, game->player.pos))
		return;
	enemy->moving = true;
	dir = vec2_unit(enemy->pos, game->player.pos);
	dir  = vec2_scale(dir, ENEMY_WALK_SPEED * game->dt);
	next.x = enemy->pos.x + dir.x;
	if (can_move(game, next.x, enemy->pos.y))
		enemy->pos.x = next.x;
	next.y = enemy->pos.y + dir.y;
	if (can_move(game, enemy->pos.x, next.y))
		enemy->pos.y = next.y;
}


void enemy_attack_player(t_game *game, t_enemy *enemy)
{
	double   dist_sq;


	dist_sq = vec2_len_squared(enemy->pos, game->player.pos);
	if (dist_sq < TILE_SIZE * TILE_SIZE * 2) 
	{
		game->player.lives--;
		if (game->player.lives <= 0)
			printf("R.I.P DARLING MOUSE\n");
		else
			printf("Oops! you still have %d lives\n", game->player.lives);
	}
}

char *get_enemy_state_string(t_enemy *enemy)
{
	switch (enemy->state)
	{
		case ENEMY_WALKING: return ("ENEMY_WALKING");
		case ENEMY_ATTACKING: return ("ENEMY_ATTACKING");
		case ENEMY_DEAD: return ("ENEMY_DEAD");
		default: return ("Unknown state");
	}
}

void enemy_update_state(t_game *game, t_enemy *enemy)
{
	t_animation *anim;

	if (enemy->state == ENEMY_DEAD || enemy->state == ENEMY_WALKING)
		return ;
	anim = enemy->animation[enemy->state];
	if (enemy->state == ENEMY_ATTACKING && !anim->finished)
	{
		if (anim->end == anim->curr)
			game->shake = 30;
	}
	else if (enemy->state == ENEMY_ATTACKING && anim->finished)
	{
		enemy_attack_player(game, enemy);
		enemy->state = ENEMY_WALKING;
		anim->finished = false;
	}
}
