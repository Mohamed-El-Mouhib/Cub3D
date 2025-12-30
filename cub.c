/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:28:28 by aljbari           #+#    #+#             */
/*   Updated: 2025/11/17 22:29:10 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

void draw_frame(t_game *game, t_data *data, double noise_x, double noise_y)
{
	unsigned int color;
	unsigned int offset;
	unsigned int ignore_color;
	int x_off;
	int y_off;

	x_off = game->scene.width - data->width + noise_x;
	y_off = game->scene.height - data->height + noise_y + 30; // 30 is for image to go down a bit
	offset = (0 * data->line_len + 0 * (data->bpp / 8));
	color = *((unsigned int *)(data->addr + offset));
	ignore_color = color;

	for (int y = 0; y < data->height; y++)
	{
		for (int x = 0; x < data->width; x++)
		{
			offset = (y * data->line_len + x * (data->bpp / 8));
			color = *((unsigned int *)(data->addr + offset));
			if (color == ignore_color)
				continue;
			image_put_pixel(&game->scene, x + x_off, y + y_off, color);
		}
	}
}

void player_render_frame(t_game *game)
{
	t_animation *anim;

	anim = game->player.animations[game->player.state];
	if (!anim->is_running || (size_t)(game->tick - anim->last_changed) < anim->duration)
	{
		draw_frame(game, dyn_at(game->assets, anim->start + anim->curr), game->player.bob.x, game->player.bob.y);
		return ;
	}
	anim->last_changed = game->tick;
	anim->curr += anim->dir;
	if (anim->curr > anim->end)
	{
		anim->dir = -1;
		anim->curr = anim->end;
	}
	if (anim->curr < anim->start)
	{
		anim->dir = +1;
		anim->curr = anim->start;
	}
	draw_frame(game, dyn_at(game->assets, anim->start + anim->curr), game->player.bob.x, game->player.bob.y);
}


void game_rander(t_game *game)
{
	raycast_draw_walls(game);
	draw_minimap(game);
	player_update_bobing(game);
	player_render_frame(game);
	mlx_put_image_to_window(game->mlx, game->win, game->scene.img, 0, 0);
}


bool is_frame_ready()
{
	static time_t last_frame_time;
	static int frames;

	if (curr_time_ms() - last_frame_time > 4000)
	{
		printf("FRAMES: %d\n", frames/4);
		last_frame_time = curr_time_ms();
		frames = 0;
	}
	frames++;
	return (true);
}

int game_loop(t_game *game)
{
	if (!is_frame_ready())
		return (0);
	game->tick = curr_time_ms();
	game_handle_keyboard_events(game);
	game_rander(game);
	return (0);
}

/*
 * prepare_wall_images: loads wall textures from files and stores them in the frames struct.
 * @tmp:   temporary variable that stores the result of the texture loading function (mlx_xpm_file_to_image).
 * @x:     variable that holds the width of the texture.
 * @y:     variable that holds the height of the texture.
 * return: this function does not return a value.
 */
void	prepare_wall_images(t_game *game)
{
	void	*tmp;
	int	x;
	int	y;
	int	bpp;
	int	endian;
	int	line_len;

	tmp = mlx_xpm_file_to_image(game->mlx, "t1.xpm", &x, &y);
	game->frames.walls[0].addr = mlx_get_data_addr(tmp,&bpp, &line_len, &endian);
	game->frames.walls[0].height = y;
	game->frames.walls[0].width = x;
	game->frames.walls[0].bpp = bpp;
	game->frames.walls[0].line_len = line_len;
	game->frames.walls[0].endian = endian;
	tmp = mlx_xpm_file_to_image(game->mlx,"t2.xpm", &x, &y);
	game->frames.walls[1].addr = mlx_get_data_addr(tmp, &bpp, &line_len, &endian);
	game->frames.walls[1].height = y;
	game->frames.walls[1].width = x;
	game->frames.walls[1].bpp = bpp;
	game->frames.walls[1].line_len = line_len;
	game->frames.walls[1].endian = endian;

}

int	main(int ac, char **av)
{
	t_game game;

	if (ac != 2)
		return 1;
	init_game(&game, av[1]);
	prepare_wall_images(&game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_hook(game.win, 06, 1L<<6, handle_mouse_event, &game); // Our mouse friend
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);         // if key pressed
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);       // if key released
	mlx_loop(game.mlx);
	return (0);
}
