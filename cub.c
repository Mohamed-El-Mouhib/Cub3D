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

void game_update_shaking(t_game *game)
{
	game->shake = lerp(game->shake, 0, 0.2);
}

void game_update_time(t_game *game)
{
	time_t ct;

	ct = curr_time_ms();
	game->dt = (ct - game->tick) / 1000.0;
	game->tick = curr_time_ms();
}

void game_update(t_game *game)
{
	// game
	game_update_time(game);
	game_update_shaking(game);

	// player
	player_update_bobbing(game);
	player_update_sway(game);
	player_update_velocity(game);
	player_update_pos(game);
	player_update_state(game);
	player_update_frame(game);

	// enemy
	enemy_update_pos(game, game->enemies->buff[0]);
	enemy_update_state(game, game->enemies->buff[0]);
	enemy_update_frame_all(game);
}

void game_rander(t_game *game)
{
	raycast_draw_walls(game);
	draw_minimap(game);
	enemy_draw_all(game);
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
	game_handle_keyboard_events(game);
	game_update(game);
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
