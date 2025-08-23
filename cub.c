/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:28:28 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 19:06:22 by your_login       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"

void game_rander(t_game *game)
{
	raycast_draw_walls(game);
	draw_minimap(game);
	mlx_put_image_to_window(game->mlx, game->win,game->scene.img, 0, 0);
}

bool is_frame_ready()
{
	static time_t last_frame_time;
	static time_t one_frame_time = 1000 / FRAME_RATE;
	time_t now;

	now = curr_time_ms();
	if (now - last_frame_time < one_frame_time)
		return (true);
	// printf("FPS: %lu\n", 1000 / (now - last_frame_time));
	last_frame_time = now;
	return (true);
}

int game_loop(t_game *game)
{
	static time_t last_frame_time;
	static int frames;

	if (curr_time_ms() - last_frame_time > 1000)
	{
		printf("FRAMES: %d\n", frames);
		last_frame_time = curr_time_ms();
		frames = 0;
	}
	// if (!is_frame_ready())
	// 	return (0);
	game_handle_keyboard_events(game);
	game_rander(game);
	frames++;
	return (0);
}

void	prepare_wall_images(t_game *game)
{
	void	*tmp;

	tmp = mlx_xpm_file_to_image(game->mlx,"t1.xpm",
		&game->frames.Dimensions[0][0], &game->frames.Dimensions[0][1]);
	game->frames.walltex_[0] = mlx_get_data_addr(tmp,&game->frames.image[0].bpp,
		&game->frames.image[0].line_len,&game->frames.image[0].endian);
	tmp = mlx_xpm_file_to_image(game->mlx,"t2.xpm",&game->frames.Dimensions[1][0],
		&game->frames.Dimensions[1][1]);
	game->frames.walltex_[1] = mlx_get_data_addr(tmp,&game->frames.image[1].bpp,
		&game->frames.image[1].line_len,&game->frames.image[1].endian);
}

int	main()
{
	t_game game;

	init_game(&game);
	prepare_wall_images(&game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_hook(game.win, 06, 1L<<6, handle_mouse_event, &game); // Our mouse friend
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);         // if key pressed
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);       // if key released
	mlx_loop(game.mlx);
	return (0);
}
