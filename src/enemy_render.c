#include "../includes/cub3d.h"
#include <stddef.h>

t_vec2 enemy_camera_pos(t_game *game, t_enemy *enemy)
{
	t_player *p = &game->player;
	t_vec2 camera_pos;
	t_vec2 p_to_enemy;
	double invDet;

	p_to_enemy = vec2_scale(vec2_sub(enemy->pos, p->pos), 1 / TILE_SIZE);
	invDet = 1.0 / (p->plane.x * p->dir.y - p->dir.x * p->plane.y);
	camera_pos.x = invDet * (p->dir.y * p_to_enemy.x - p->dir.x * p_to_enemy.y);
	camera_pos.y = invDet * (-p->plane.y * p_to_enemy.x + p->plane.x * p_to_enemy.y);
	return (camera_pos);
}

/**
 *    --------------------------------
 *    |        This --> -----------  |
 *    |                 |         |  |
 *    |                 |  enemy  |  |
 *    |                 |         |  |
 *    |                 -----------  |
 *    -------------------------------- <- Screen
 *
 * Returns the postistion of the point referred to in the figure
 */
t_vec2 enemy_set_start(t_game *game, t_enemy *enemy)
{
	t_vec2 start;

	start.x = enemy->screen - (int)(enemy->size / 2);
	if (start.x < 0)
		start.x = 0;
	start.y = (int)((game->screen_height - enemy->size) / 2);
	if (start.y < 0)
		start.y = 0;
	return (start);
}

/**
 *    --------------------------------
 *    |  -----------                  |
 *    |  |         |                  |
 *    |  |  Enemy  |                  |
 *    |  |         |                  |
 *    |  ----------- <-- This         |
 *    --------------------------------- <- Screen
 *
 * Return: The postistion of the point referred to in the figure
 */
t_vec2 enemy_set_end(t_game *game, t_enemy *enemy)
{
	t_vec2 bottom;

	bottom.x = (int)(enemy->size / 2) + enemy->screen;
	if(bottom.x >= (int)game->screen_width)
		bottom.x = game->screen_width - 1;
	bottom.y = (int)((enemy->size + game->screen_height) / 2);
	if(bottom.y >= (int)game->screen_height)
		bottom.y = game->screen_height - 1;
	return (bottom);
}

static unsigned int get_point_color(t_game *game, t_enemy *enemy, int i, int j)
{
	int tex_w;
	int tex_h;
	int x;
	int y;
	t_data *frame;

	frame = animation_get_frame(game, enemy->animation[enemy->state]);
	tex_w = frame->width;
	tex_h = frame->height;
	if (enemy->s.x)
		x = (i - enemy->s.x) / enemy->size * tex_w;
	else
		x = (i - enemy->s.x + enemy->size - enemy->e.x) / enemy->size * tex_w;

	y = (j - enemy->s.y) / enemy->size * tex_h;
	return (imgae_get_pixel(frame, x, y));
}

void draw_enemy_frame(t_game *game, t_enemy *enemy)
{
	unsigned int color;
	unsigned int ignore_color;
	t_data *frame;

	frame = animation_get_frame(game, enemy->animation[enemy->state]);
	ignore_color = *(unsigned int *)frame->addr;
	for (int i = enemy->s.x; i < enemy->e.x; i++)
	{
		if (enemy->camera.y > game->stripes[i] / TILE_SIZE)
			continue;
		for (int j = enemy->s.y; j < enemy->e.y; j++)
		{
			color = get_point_color(game, enemy, i, j);
			if (ignore_color == color)
				continue;
			image_put_pixel(&game->scene, i, j, color);
		}
	}
}

void draw_enemy_borders(t_game *game, t_enemy *enemy)
{
	t_vec2 p1;
	t_vec2 p2;
	t_vec2 p3;
	t_vec2 p4;

	p1 = vec2_new(enemy->s.x, enemy->s.y);
	p2 = vec2_new(enemy->e.x, enemy->s.y);
	p3 = vec2_new(enemy->s.x, enemy->e.y);
	p4 = vec2_new(enemy->e.x, enemy->e.y);
	draw_line(&game->scene, p1, p2, COLOR_RED);
	draw_line(&game->scene, p1, p3, COLOR_RED);
	draw_line(&game->scene, p4, p2, COLOR_RED);
	draw_line(&game->scene, p4, p3, COLOR_RED);
}

void draw_enemy(t_game *game, t_enemy *enemy)
{
	int w = game->screen_width;
	int h = game->screen_height;
	enemy->camera = enemy_camera_pos(game, enemy);
	enemy->screen = (int)(((double)w / 2) * (1 + enemy->camera.x / enemy->camera.y));
	enemy->size = abs((int)(h / (enemy->camera.y)));
	enemy->s = enemy_set_start(game, enemy);
	enemy->e = enemy_set_end(game, enemy);
	if (enemy->camera.y < 0 ||  enemy->size > 1500)
		return ;
	if (enemy->screen > w + enemy->size)
		return ;
	draw_enemy_frame(game, enemy);
	draw_enemy_borders(game, enemy);
}

void draw_enemies(t_game *game)
{
	t_enemy *enemy;

	for (size_t i = 0; i < game->enemies->length; i++)
	{
		enemy = dyn_at(game->enemies, i);
		animation_cycle(game, enemy->animation[enemy->state]);
		draw_enemy(game, dyn_at(game->enemies, i));
	}
}
