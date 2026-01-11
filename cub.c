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
#include "includes/graphics.h"

void game_update_shaking(t_game *game)
{
	game->shake = lerp(game->shake, 0, 0.2);
}

void draw_digit(t_game *game, t_data *image, int x_off, int y_off, int number)
{
	unsigned int ignore_color; 
	unsigned int color; 
	int x;
	int y;
	int start;

	if (!image)
		return ;
	ignore_color = image_get_pixel(image, 0, 0);
	y = 0;
	while (y < image->height)
	{
		x = start = 45 * number;
		while (x < image->width && x < start + 45)
		{
			color = image_get_pixel(image, x, y);
			if (color != ignore_color)
				image_put_pixel(&game->scene, x + x_off - start, y + y_off, color);
			x++;
		}
		y++;
	}
}

void put_number(t_game *game, t_vec2 pos, int number)
{
	int digits[15];
	int i = 0;
	int j = 0;

	digits[i] = 0;
	while (number > 0)
	{
		digits[i++] = number % 10;
		number /= 10;
	}
	if (i > 0)
		i--;
	while (i >= 0)
	{
		draw_digit(game, game->numbers, pos.x + j * 45, pos.y, digits[i]);
		j++;
		i--;
	}
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
	// t_enemy *enemy = game->enemies->buff[0];
	// printf("FRAME Id: %zu\n", enemy->animation[enemy->state]->curr);
}

typedef struct s_color {
	unsigned char r:8;
	unsigned char g:8;
	unsigned char b:8;
	unsigned char a:8;
} t_color_;


void game_rander(t_game *game)
{
	raycast_draw_walls(game);
	draw_minimap(game);
	enemy_draw_all(game);
	player_render_frame(game);
	draw_filled_circle(&game->scene, vec2_new(game->screen_width / 2.0, game->screen_height / 2.0), 8, COLOR_RED);
	put_number(game,vec2_new(10, game->screen_height - 80), game->player.ammo);
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
	game_handle_inputs(game);
	game_update(game);
	game_rander(game);
	return (0);
}

void	release_resources(t_game* game, int i)
{
	while (i >= 0)
	{
		mlx_destroy_image(game->mlx, &game->wall[i]);
		--i;
	}
}

void	init_texture_assets(t_game *game)
{
	int	i;
	char	*path;

	i = -1;
	while (++i < 2)
	{
		path = info()->ptr[i];
		if (!image_load_xpm(game, &game->wall[i], path))
		{
			release_resources(game, i);
			exit(1);
		}
	}
}

int	main(int ac, char **av)
{
	t_game game;

	if (ac != 2)
		return 1;
	init_game(&game, av[1]);
	init_texture_assets(&game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_hook(game.win, 6, 1L << 6, handle_mouse_move, &game); // Our mouse friend
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);         // if key pressed
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);       // if key released
	mlx_hook(game.win, 4, 1L << 2, handle_mouse_press, &game);
	mlx_hook(game.win, 5, 1L << 3, handle_mouse_release, &game);
	mlx_loop(game.mlx);
	return (0);
}
