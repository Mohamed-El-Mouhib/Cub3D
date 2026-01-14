#include "../includes/cub3d.h"

/**
 * enemy_camera_pos - Calculate the position of the player in the coordinate
 * system: (player->dir, player->plane)
 */
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
t_vec2 enemy_get_drawing_start(t_game *game, t_enemy *enemy)
{
	t_vec2 start;

	start.x = enemy->screen - (int)(enemy->size / 2);
	// if (start.x < 0)
	// 	start.x = 0;
	start.y = (int)((game->screen_height - enemy->size) / 2);
	// if (start.y < 0)
	// 	start.y = 0;
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
t_vec2 enemy_get_drawing_end(t_game *game, t_enemy *enemy)
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

/**
 * get_point_color - get the pixel color of the point (i, j)
 */
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
	x = (i - enemy->s.x) / enemy->size * tex_w;
	y = (j - enemy->s.y) / enemy->size * tex_h;

	return (image_get_pixel(frame, x, y));
}

/**
 * enemy_draw_frame - draw the current frame of the enemy in the correct
 * position
 */
void enemy_draw_frame(t_game *game, t_enemy *enemy)
{
	t_color color;
	t_color ignore_color;
	t_data *frame;

	frame = animation_get_frame(game, enemy->animation[enemy->state]);
	ignore_color = *(unsigned int *)frame->addr;
	for (int x = enemy->s.x; x < enemy->e.x; x++)
	{
		if (x < 0 || enemy->camera.y > game->stripes[x] / TILE_SIZE)
			continue;
		for (int y = enemy->s.y; y < enemy->e.y; y++)
		{
			if (y < 0)
				continue;
			color = get_point_color(game, enemy, x, y);
			if (ignore_color == color)
				continue;
			image_put_pixel(&game->scene, x, y, color);
		}
	}
}

/**
 * enemy_draw_borders - draw enemy borders to show its boundaries
 */
void enemy_draw_borders(t_game *game, t_enemy *enemy)
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

/**
 * enemy_draw_one - Calculate the enemy posistion in the screen then draw its
 * current frame
 */
void enemy_draw_one(t_game *game, t_enemy *enemy)
{
	int w = game->screen_width;
	int h = game->screen_height;
	enemy->camera = enemy_camera_pos(game, enemy);
	enemy->screen = (int)(((double)w / 2) * (1 + enemy->camera.x / enemy->camera.y));
	enemy->size = abs((int)(h / (enemy->camera.y)));
	enemy->s = enemy_get_drawing_start(game, enemy);
	enemy->e = enemy_get_drawing_end(game, enemy);
	if (enemy->camera.y < 0 ||  enemy->size > 1500)
		return ;
	if (enemy->screen > w + enemy->size)
		return ;
	enemy_draw_frame(game, enemy);
}

/**
 * enemy_update_frame_all - Update the current frame of each enemy in the game
 */
void enemy_update_frame_all(t_game *game)
{
	t_enemy *enemy;

	for (size_t i = 0; i < game->enemies->length; i++)
	{
		enemy = dyn_at(game->enemies, i);
		animation_cycle(game, enemy->animation[enemy->state]);
	}
}

/**
 * enemy_draw_all - draw the current frame of each enemy in the game
 */
void enemy_draw_all(t_game *game)
{
	size_t i;

	i = 0;
	while (i < game->enemies->length)
	{
		enemy_draw_one(game, dyn_at(game->enemies, i++));
	}
}
