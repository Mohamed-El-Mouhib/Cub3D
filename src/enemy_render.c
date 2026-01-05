#include "../includes/cub3d.h"

t_vec2 enemy_camera_pos(t_game *game)
{
	t_player *p = &game->player;
	t_vec2 camera_pos;
	t_vec2 p_to_enemy;
	double invDet;

	p_to_enemy = vec2_scale(vec2_sub(game->enemy.pos, p->pos), 1 / TILE_SIZE);
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
t_vec2 enemy_start_point(t_game *game, int spriteSize, double cameraPosX)
{
	t_vec2 start;

	start.x = cameraPosX - (int)(spriteSize / 2);
	if (start.x < 0)
		start.x = 0;
	start.y = (int)((game->screen_height - spriteSize) / 2);
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
t_vec2 enemy_end_point(t_game *game, int spriteSize, double cameraPosX)
{
	t_vec2 bottom;

	bottom.x = (int)(spriteSize / 2) + cameraPosX;
	if(bottom.x >= (int)game->screen_width)
		bottom.x = game->screen_width - 1;
	bottom.y = (int)((spriteSize + game->screen_height) / 2);
	if(bottom.y >= (int)game->screen_height)
		bottom.y = game->screen_height - 1;
	return (bottom);
}

void get_color_helper(t_game *game, int i, int j, t_vec2 s, t_vec2 e, int spriteSize)
{
	int tex_w;
	int tex_h;

	tex_w = game->enemy.frame->width;
	tex_h = game->enemy.frame->height;
	int x = (double)(i - s.x + (s.x ? 0 : spriteSize - e.x)) / spriteSize * tex_w;
	int y = (double)(j - s.y) / spriteSize * tex_h;
	int offset = (y * game->enemy.frame->line_len + x * (game->enemy.frame->bpp / 8));
	unsigned int color = *((unsigned int *)(game->enemy.frame->addr + offset));
}

void draw_enemy_frame(t_game *game, t_vec2 camera, t_vec2 s, t_vec2 e, int spriteSize)
{
	int tex_w;
	int tex_h;

	tex_w = game->enemy.frame->width;
	tex_h = game->enemy.frame->height;
	for (int i = s.x; i < e.x; i++)
	{
		if (camera.x > game->stripes[i] / TILE_SIZE)
			continue;
		for (int j = s.y; j < e.y; j++)
		{
			int x = (double)(i - s.x + (s.x ? 0 : spriteSize - e.x)) / spriteSize * tex_w;
			int y = (double)(j - s.y) / spriteSize * tex_h;
			int offset = (y * game->enemy.frame->line_len + x * (game->enemy.frame->bpp / 8));
			unsigned int color = *((unsigned int *)(game->enemy.frame->addr + offset));
			if (*(unsigned int *)game->enemy.frame->addr == color)
				continue;
			image_put_pixel(&game->scene, i, j, color);
		}
	}
}

void draw_enemy_borders(t_game *game, t_vec2 s, t_vec2 e)
{
	t_vec2 p1;
	t_vec2 p2;
	t_vec2 p3;
	t_vec2 p4;

	p1 = vec2_new(s.x, s.y);
	p2 = vec2_new(e.x, s.y);
	p3 = vec2_new(s.x, e.y);
	p4 = vec2_new(e.x, e.y);
	draw_line(&game->scene, p1, p2, COLOR_RED);
	draw_line(&game->scene, p1, p3, COLOR_RED);
	draw_line(&game->scene, p4, p2, COLOR_RED);
	draw_line(&game->scene, p4, p3, COLOR_RED);
}

void draw_enemy(t_game *game)
{
	int w = game->screen_width;
	int h = game->screen_height;
	t_vec2 camera = enemy_camera_pos(game);
	t_vec2 s; // start
	t_vec2 e; // end
	int cameraPosX = (int)(((double)w / 2) * (1 + camera.x / camera.y));
	int spriteSize = abs((int)(h / (camera.y)));

	s = enemy_start_point(game, spriteSize, cameraPosX);
	e = enemy_end_point(game, spriteSize, cameraPosX);

	if (camera.y < 0 || cameraPosX < -spriteSize || cameraPosX > w + spriteSize || spriteSize > 1500)
		return ;

	draw_enemy_frame(game, camera, s, e, spriteSize);
	draw_enemy_borders(game, s, e);
}
