
#include "includes/cub3d.h"
#include "includes/graphics.h"
#include "includes/types.h"
#include <fcntl.h>


time_t	curr_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int map[MAP_HEIGHT][MAP_WIDTH] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 0, 0, 0, 0, 0, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 1, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

t_vec2 intersect;


void ft_exit_error(char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));	
	exit(1);
}

void init_game(t_game *game)
{
	game->mlx = mlx_init();
	if (!game->mlx)
		ft_exit_error("Faild to allocate mlx");

	game->world.map_width = MAP_WIDTH;
	game->world.map_height = MAP_HEIGHT;
	game->world.map = map;
	game->screen_width = MAP_WIDTH * TILE_SIZE;
	game->screen_height = MAP_HEIGHT * TILE_SIZE;
	game->win = mlx_new_window(game->mlx, game->screen_width, game->screen_height, "Hello world!");
	if (!game->win)
		ft_exit_error("Faild to allocate window");

	game->scene = image_new(game, game->screen_width, game->screen_height);
	game->mouse_pos = vec2_new(0, 0);
	game->player.pos = vec2_new(game->screen_width / 2.0, game->screen_height / 2.0);
	// player looking up
	game->player.dir = vec2_new(-1, 0);
	// field of view is 60 degree 
	game->player.plane = vec2_new(0, 0.66);

	// The rotation angle calcualte the cos,sin once to cache it for reuse later
	game->player.rot_angle = INIT_ROTATION_STEP;
	game->player.rot_cos = cos(INIT_ROTATION_STEP);
	game->player.rot_sin = sin(INIT_ROTATION_STEP);
	ft_bzero(game->keyboard_events, sizeof(game->keyboard_events));
}

typedef enum e_rotate_dir
{
	ROTATE_LEFT  = 1,
	ROTATE_RIGHT = -1
} t_rotate_dir;

typedef enum e_move_dir
{
	MOVE_FORWARD,
	MOVE_BACKWARD,
	MOVE_LEFT,
	MOVE_RIGHT,
} t_move_dir;

/**
 * player_rotate - Rotates the player's direction vector by the player's
 * rotation angle
 *
 * this is the formula that have been:
 *     | x |    | cos(a)  -sin(a) |    | x |
 *     | y | =  | sin(a)   cos(a) | *  | y |
 * @player: Pointer to the player containing direction vector and rotation values
 * @direction: the rotation direction use ROTATE_LEFT or ROTATE_RIGHT macros
 */
void player_rotate(t_player *player, t_rotate_dir rot_dir)
{
	t_vec2 dir;
	t_vec2 plane;

	dir.x = player->dir.x * player->rot_cos - player->dir.y * player->rot_sin * rot_dir;
	dir.y = player->dir.x * player->rot_sin * rot_dir + player->dir.y * player->rot_cos;
	plane.x = player->plane.x * player->rot_cos - player->plane.y * player->rot_sin * rot_dir;
	plane.y = player->plane.x * player->rot_sin * rot_dir + player->plane.y * player->rot_cos;
	player->dir = dir;
	player->plane = plane;
}

void player_move(t_player *player, int speed, t_move_dir move_dir)
{
	if (move_dir == MOVE_LEFT)
		player->pos.x += player->dir.x * speed * -1;
	else if (move_dir == MOVE_RIGHT) 
		player->pos.x += player->dir.x * speed * 1;
	else if (move_dir == MOVE_BACKWARD)
	{
		player->pos.x += player->dir.x * speed * -1;
		player->pos.y += player->dir.y * speed * -1;
	}
	else
	{
		player->pos.x += player->dir.x * speed * 1;
		player->pos.y += player->dir.y * speed * 1;
	}
}


time_t time = 0;

enum {
	KEY_ESCAPE,
	KEY_RIGHT,
	KEY_LEFT,
	KEY_DOWN,
	KEY_UP,
	KEY_W,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_R,
};

int	key_release(int key_code, t_game *game)
{
	if (key_code == XK_Escape)
		exit(0);
	else if (key_code == XK_Right)
		game->keyboard_events[KEY_RIGHT] = false;
	else if (key_code == XK_Left)
		game->keyboard_events[KEY_LEFT] = false;
	else if (key_code == 'w')
		game->keyboard_events[KEY_W] = false;
	else if (key_code == 's')
		game->keyboard_events[KEY_S] = false;
	else if (key_code == 'r')
		game->keyboard_events[KEY_R] = false;
	else if (key_code == 'd')
		game->keyboard_events[KEY_D] = false;
	else if (key_code == 'a')
		game->keyboard_events[KEY_A] = false;
	return (0);
}

int	key_press(int key_code, t_game *game)
{
	if (key_code == XK_Escape)
		exit(0);
	else if (key_code == XK_Right)
		game->keyboard_events[KEY_RIGHT] = true;
	else if (key_code == XK_Left)
		game->keyboard_events[KEY_LEFT] = true;
	else if (key_code == 'w')
		game->keyboard_events[KEY_W] = true;
	else if (key_code == 's')
		game->keyboard_events[KEY_S] = true;
	else if (key_code == 'r')
		game->keyboard_events[KEY_R] = true;
	else if (key_code == 'd')
		game->keyboard_events[KEY_D] = true;
	else if (key_code == 'a')
		game->keyboard_events[KEY_A] = true;
	return (0);
}

void handle_input_events(t_game *game)
{
	t_player *player;

	player = &game->player;
	if (game->keyboard_events[KEY_RIGHT])
		player_rotate(player, ROTATE_RIGHT);
	if (game->keyboard_events[KEY_LEFT])
		player_rotate(player, ROTATE_LEFT);
	if (game->keyboard_events[KEY_W])
		player_move(player, 2, MOVE_FORWARD);
	if (game->keyboard_events[KEY_S])
		player_move(player, 2, MOVE_BACKWARD);
	if (game->keyboard_events[KEY_R])
		player->pos = vec2_new(0, 0);
	if (game->keyboard_events[KEY_D])
		player_move(player, 2, MOVE_RIGHT);
	if (game->keyboard_events[KEY_A])
		player_move(player, 2, MOVE_LEFT);
}

#define MINIMAP_SCALE 4.0

void minimap_draw_player(t_game *game)
{
	double scale = TILE_SIZE / MINIMAP_SCALE;
	t_vec2 head;
	t_vec2 mini_player_pos;


	mini_player_pos = vec2_div(game->player.pos, MINIMAP_SCALE);
	head = vec2_add(vec2_scale(game->player.dir, scale), mini_player_pos);
	draw_line(&game->scene, mini_player_pos, head, MINDARO);
	draw_filled_circle(&game->scene, mini_player_pos, 2, WHITE);
}

void minimap_daw_grid(t_data *buff)
{
	t_vec2 start;
	int i;
	int grid_sz = TILE_SIZE / MINIMAP_SCALE;

	i = 0;
	while (i < MAP_HEIGHT)
	{
		int j = 0;
		while (j < MAP_WIDTH)
		{
			if (map[i][j])
				draw_filled_square(buff, vec2_new(j * grid_sz,i * grid_sz), grid_sz, RED);
			j++;
		}
		i++;
	}
	start = vec2_new(0, 0);
	i = 0;
	while (i <= MAP_HEIGHT)
	{
		draw_horizontal_line(buff, start, MAP_WIDTH * grid_sz, RED);
		start.y += grid_sz;
		i++;
	}
	start = vec2_new(0, 0);
	i = 0;
	while (i <= MAP_WIDTH)
	{
		draw_vertical_line(buff, start, MAP_HEIGHT * grid_sz, RED);
		start.x += grid_sz;
		i++;
	}
}
void draw_minimap(t_game *game)
{
	minimap_daw_grid(&game->scene);
	minimap_draw_player(game);
}

int handle_mouse_event(int x,int y, t_game *game)
{
	game->mouse_pos.x = x;
	game->mouse_pos.y = y;
	return (0);
}

typedef struct s_dda_alog
{
	t_vec2 delta_dist;
	t_vec2 side_dist;
	t_vec2 step_dir;
	double ray_size;
	double hit_dist;
	t_vec2 map_pos;
	t_vec2 ray;
	int side;
	t_vec2 wall_top;
	t_vec2 wall_bottom;
} t_dda_algo;


double sign(double x)
{
	return ((x > 0) - (x < 0));
}

/**
 * init_delta_dist - initialize ray delta distances
 * @dda: pointer to dda algorithm context
 *
 * Delta distances represent the length a ray has to travel in order to
 * move exactly one tile unit in the x or y direction
 */
void init_delta_dist(t_dda_algo *dda)
{
	if (dda->ray.x != 0)
		dda->delta_dist.x = (fabs(1.0 / dda->ray.x) * TILE_SIZE);
	else
		dda->delta_dist.x = 1e30;
	if (dda->ray.y != 0)
		dda->delta_dist.y = (fabs(1.0 / dda->ray.y) * TILE_SIZE);
	else
		dda->delta_dist.y = 1e30;
}

/**
 * find_first_hitting_wall - run DDA loop until a wall or map edge is hit
 * This will aslo set the side of wall
 *
 * @game: pointer to game context
 * @dda: pointer to dda algorithm context
 *
 */
void find_first_hitting_wall(t_game *game, t_dda_algo *dda)
{
	int stop = 0;

	while (!stop)
	{
		if (dda->side_dist.x < dda->side_dist.y)
		{
			dda->side_dist.x += (dda->delta_dist.x * TILE_SIZE);
			dda->map_pos.x += (int)dda->step_dir.x;
			dda->side = 0;
		}
		else
		{
			dda->side_dist.y += (dda->delta_dist.y * TILE_SIZE);
			dda->map_pos.y += (int)dda->step_dir.y;
			dda->side = 1;
		}
		if (dda->map_pos.x < 0 || dda->map_pos.x >= MAP_WIDTH || dda->map_pos.y < 0 || dda->map_pos.y >= MAP_HEIGHT)
			stop = 1;
		if ((game->world.map)[(int)dda->map_pos.y][(int)dda->map_pos.x])
			stop = 1;
	}
}

/**
 * init_side_dist_and_step - compute step direction and initial side distance
 * - side distances (distance to first x/y wall grid)
 * - step direction (which way the we will move in the map)
 *
 * @player: pointer to player
 * @dda: pointer to dda algorithm context
 */
void init_side_dist_and_step(t_player *player, t_dda_algo *dda)
{
	if (dda->ray.x < 0)
	{
		dda->side_dist.x = dda->delta_dist.x * (player->pos.x - dda->map_pos.x * TILE_SIZE);
		dda->step_dir.x = -1.0;
	}
	else
	{
		dda->side_dist.x = dda->delta_dist.x * ((dda->map_pos.x + 1) * TILE_SIZE - player->pos.x);
		dda->step_dir.x = 1.0;
	}
	if (dda->ray.y < 0)
	{
		dda->side_dist.y = dda->delta_dist.y * (player->pos.y - dda->map_pos.y * TILE_SIZE);
		dda->step_dir.y = -1.0;
	}
	else
	{
		dda->side_dist.y = dda->delta_dist.y * ((dda->map_pos.y + 1) * TILE_SIZE - player->pos.y);
		dda->step_dir.y = 1.0;
	}
}

/**
 * init_map_pos - set the position of player in the grip map
 *
 * @player: pointer to player
 * @dda: pointer to dda algorithm context
 */
void dda_init_map_pos(t_player *player, t_dda_algo *dda)
{
	dda->map_pos.x = (int) (player->pos.x / TILE_SIZE);
	dda->map_pos.y = (int) (player->pos.y / TILE_SIZE);
}

/**
 * compute_ray_dir - calculates ray direction based on player direction and
 * calculate current camera plane slice
 *
 * @game: pointer to game context
 * @dda: pointer to dda algorithm context
 * @x: current screen column
 */
void compute_ray_dir(t_game *game, t_dda_algo *dda, double x)
{
	dda->ray_size = 2.0 * x / game->screen_width - 1;
	dda->ray = vec2_add(game->player.dir, vec2_scale(game->player.plane, dda->ray_size));
}

/**
 * compute_dist_to_hit_wall - compute perpendicular distance to the hit wall
 *
 * @dda: pointer to dda algorithm context
 */
void compute_dist_to_hit_wall(t_dda_algo *dda)
{
	if(dda->side == 0)
		dda->hit_dist = (dda->side_dist.x  / TILE_SIZE - dda->delta_dist.x);
	else
		dda->hit_dist = (dda->side_dist.y  / TILE_SIZE - dda->delta_dist.y);
}

/**
 * compute_wall_boundaries - calculate wall slice screen coordinates
 *
 * @game: pointer to game context
 * @dda: pointer to dda algorithm context
 * @x: current screen column
 */
void calculate_wall_boundaries(t_game *game, t_dda_algo *dda, int x)
{
	double line_height;
	double h;

	line_height = game->screen_height / dda->hit_dist * TILE_SIZE;
	h = game->screen_height;
	dda->wall_top.y = (h - line_height) / 2;
	if(dda->wall_top.y < 0)
		dda->wall_top.y = 0;
	dda->wall_bottom.y = (line_height + h) / 2;
	if(dda->wall_bottom.y >= h)
		dda->wall_bottom.y = h - 1;
	dda->wall_top.x = x;
	dda->wall_bottom.x = x;
}

/**
 * perform_dda - run all DDA initialization and stepping
 *
 * @game: pointer to game context
 * @dda: pointer to dda algorithm context
 * @x: screen column
 *
 * High-level function to compute ray, run DDA, and calculate hit distance, for
 * every time this function runs, you'll find all the necessary info in the dda
 * context, changing the x, means changing the current column
 */
void perform_dda(t_game *game, t_dda_algo *dda, int x)
{
	dda_init_map_pos(&game->player, dda);
	compute_ray_dir(game, dda, x);
	init_delta_dist(dda);
	init_side_dist_and_step(&game->player, dda);
	find_first_hitting_wall(game, dda);
	compute_dist_to_hit_wall(dda);
}

void draw_fov_in_minimap(t_game *game, t_dda_algo *dda)
{
	t_vec2 mini_player_pos; // the player position in the minimap
	t_vec2 mini_hit_point;  // where the ray hits in minimap?

	mini_player_pos = vec2_div(game->player.pos, MINIMAP_SCALE);
	mini_hit_point = vec2_add(mini_player_pos, 
			   vec2_scale(dda->ray, dda->hit_dist / MINIMAP_SCALE)
			   );
	draw_line(&game->scene, 
	   mini_player_pos, 
	   mini_hit_point
	   ,MINDARO);
}

void your_beutiful_randring_logic(t_game *game, t_dda_algo *dda)
{
	if (dda->side)
		draw_line(&game->scene, dda->wall_top, dda->wall_bottom, PURPLE);
	else
		draw_line(&game->scene, dda->wall_top, dda->wall_bottom, TINY_BLACK);
}

void draw_walls(t_game *game)
{
	t_dda_algo dda;

	for (int x = 0; x < (int)game->screen_width; x++)
	{
		perform_dda(game, &dda, x);
		if ((int)dda.hit_dist != 0)
		{
			calculate_wall_boundaries(game, &dda, x);
			draw_fov_in_minimap(game, &dda);
			your_beutiful_randring_logic(game, &dda);
		}
	}
}



int game_loop(t_game *game)
{
	static time_t last_frame_time;

	if (curr_time_ms() - last_frame_time < (1000 / 60))
		return (1);
	printf("Frame rate: %ld\n", 1000 / (curr_time_ms() - last_frame_time));
	last_frame_time =  curr_time_ms();
	image_clear(&game->scene);
	handle_input_events(game);
	draw_walls(game);
	draw_minimap(game);
	mlx_put_image_to_window(game->mlx, game->win,game->scene.img, 0, 0);
	return (0);
}

int	main()
{
	t_game game;


	init_game(&game);

	mlx_loop_hook(game.mlx, game_loop, &game);
	// mlx_hook(game.win, 02, 1L<<0, key_even_handler, &game);
	// mlx_hook(game.win, 03, 1L<<1, key_even_handler, &game);
	// mlx_hook(game.win, 06, 1L<<6, handle_mouse_event, &game); // for mouse
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);

	mlx_loop(game.mlx);
	return (0);
}
