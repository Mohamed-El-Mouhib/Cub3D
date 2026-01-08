#include "../includes/cub3d.h"

static char get_map_cell(t_game *game, int x, int y)
{
	if (x > (int)game->world.map_width || y > (int)game->world.map_height)
		return ('1');
	return (game->world.map[y][x]);
}


/**
 * has_los - chack if point @from has line of sight into point @to
 */
int has_los(t_game *g, t_vec2 from, t_vec2 to)
{
	t_vec2  d;
	t_vec2  curr;
	float   dist;
	int     steps;

	d = vec2_sub(to, from);
	dist = sqrt(vec2_len_squared(to, from));
	// if (dist > 300) return (0);

	d = vec2_scale(d, (TILE_SIZE / 2.0) / dist); 
	steps = (int)(dist / (TILE_SIZE / 2.0));
	curr = from;

	while (steps-- > 0)
	{
		curr = vec2_add(curr, d);
		if (get_map_cell(g, (int)curr.x / TILE_SIZE, (int)curr.y / TILE_SIZE) == '1')
			return (0);
	}
	return (1);
}


/**
 * Checks collision with a radius buffer to prevent clipping
 */
int can_move(t_game *g, float x, float y)
{
	int pad;

	pad = TILE_SIZE / 4;
	if (get_map_cell(g, (x + pad) / TILE_SIZE, (y + pad) / TILE_SIZE) == '1')
		return 0;
	if (get_map_cell(g, (x - pad) / TILE_SIZE, (y + pad) / TILE_SIZE) == '1')
		return 0;
	if (get_map_cell(g, (x + pad) / TILE_SIZE, (y - pad) / TILE_SIZE) == '1')
		return 0;
	if (get_map_cell(g, (x - pad) / TILE_SIZE, (y - pad) / TILE_SIZE) == '1')
		return 0;
	return (1);
}

void enemy_update_pos(t_game *g, t_enemy *e)
{
	t_vec2  dir;
	t_vec2  next;
	float   dist_sq;

	dist_sq = vec2_len_squared(e->pos, g->player.pos);
	if (dist_sq < TILE_SIZE * TILE_SIZE) 
	{
		if (g->tick - e->last_attack_time > 4000)
		{
			e->state = ENEMY_ATTACKING;
			e->last_attack_time = g->tick;
		}
		return;
	}
	if (!has_los(g, e->pos, g->player.pos))
		return;
	dir = vec2_unit(e->pos, g->player.pos);
	dir  = vec2_scale(dir, ENEMY_WALK_SPEED * g->dt);
	next.x = e->pos.x + dir.x;
	if (can_move(g, next.x, e->pos.y))
		e->pos.x = next.x;
	next.y = e->pos.y + dir.y;
	if (can_move(g, e->pos.x, next.y))
		e->pos.y = next.y;
}


void enemy_attack_player(t_game *g, t_enemy *e)
{
	double   dist_sq;


	dist_sq = vec2_len_squared(e->pos, g->player.pos);
	if (dist_sq < TILE_SIZE * TILE_SIZE && g->shake > 15) 
	{
		printf("Oops: You got hit\n");
	}
}

void enemy_update_state(t_game *g, t_enemy *e)
{
	t_animation *anim;

	enemy_attack_player(g, e);
	if (e->state == ENEMY_WALKING)
		return ;
	anim = e->animation[ENEMY_ATTACKING];
	if (e->state == ENEMY_ATTACKING && anim->curr != anim->end)
	{
		if (anim->end - anim->curr == 3)
			g->shake = 30;
		return ;
	}
	else
	{
		e->state = ENEMY_WALKING;
		anim->curr = anim->start;
	}
}
