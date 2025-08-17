
#include "includes/cub3d.h"
#include "includes/graphics.h"
#include "includes/types.h"
#include <fcntl.h>

void game_rander(t_game *game)
{
	image_clear(&game->scene);
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
		return (false);
	printf("FPS: %lu\n", 1000 / (now - last_frame_time));
	last_frame_time = now;
	return (true);
}

int game_loop(t_game *game)
{
	if (!is_frame_ready())
		return (0);
	game_handle_keyboard_events(game);
	game_rander(game);
	return (0);
}

int	main()
{
	t_game game;

	init_game(&game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_hook(game.win, 06, 1L<<6, handle_mouse_event, &game); // Our mouse fiend
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);   // if key pressed
	mlx_hook(game.win, 3, 1L << 1, key_release, &game); // if key released
	mlx_loop(game.mlx);
	return (0);
}
