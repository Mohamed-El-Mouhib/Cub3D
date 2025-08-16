
#include "includes/cub3d.h"
#include "includes/graphics.h"
#include "includes/types.h"


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

int	key_even_handler(int key_code, t_game *game)
{
	t_player *player;

	player = &game->player;
	if (key_code == XK_Escape)
		exit(0);
	else if (key_code == XK_Right)
		player_rotate(player, ROTATE_RIGHT);
	else if (key_code == XK_Left)
		player_rotate(player, ROTATE_LEFT);
	else if (key_code == 'w')
		player_move(player, 1, MOVE_FORWARD);
	else if (key_code == 's')
		player_move(player, 1, MOVE_BACKWARD);
	else if (key_code == 'r')
		player->pos = vec2_new(0, 0);
	else if (key_code == 'd')
		player_move(player, 1, MOVE_RIGHT);
	else if (key_code == 'a')
		player_move(player, 1, MOVE_LEFT);
	return (0);
}

#define MINIMAP_SCALE 4.0

void draw_grid(t_data  *buff)
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

int handle_mouse_event(int x,int y, t_game *game)
{
	game->mouse_pos.x = x;
	game->mouse_pos.y = y;
	return (0);
}

double (*rounding_func(double dx))(double)
{
	if (dx >= 0)
		return (floor);
	else
		return (ceil);
}

double sign(double x)
{
	return ((x > 0) - (x < 0));
}

bool out_of_boundries(t_data *scene, t_vec2 a)
{
	return (a.x < 0 || a.y < 0 || a.x > scene->width || a.y > scene->height);
}

void	intersection_points(t_data *scene, t_vec2 p1, t_vec2 p2)
{
	double k;
	double c;
	t_vec2 sub;
	double x,y;
	t_vec2 intersect;
	t_vec2 next_x, next_y;

	sub = vec2_sub(p1, p2);
	if (sub.x == 0)
		k = 0;
	else
		k = sub.y / sub.x;
	c = p1.y - k*p1.x;
	// p2.x -= (0.1 * (sign(sub.x)));
	// p2.y -= (0.1 * (sign(sub.y)));
	x = rounding_func(sub.x)(p2.x / TILE_SIZE)*TILE_SIZE;
	y = k*x + c;
	next_x = vec2_new(x, y);
	y = rounding_func(sub.y)(p2.y / TILE_SIZE)*TILE_SIZE;
	if (k != 0)
		x = (y - c) / k;
	else
		x = c;
	next_y = vec2_new(x, y);
	double len_x = vec2_len_squared(next_x, p2);
	double len_y = vec2_len_squared(next_y, p2);
	intersect = len_x > len_y ? next_y : next_x;
	draw_circle(scene, intersect, 6, PURPLE);
	return ;
}

t_vec2 init_delta_dist(t_vec2 ray)
{
	t_vec2 delta_dist;


	if (ray.x != 0)
		delta_dist.x = (fabs(1.0 / ray.x) * TILE_SIZE);
	else
		delta_dist.x = 1e30;
	if (ray.y != 0)
		delta_dist.y = (fabs(1.0 / ray.y) * TILE_SIZE);
	else
		delta_dist.y = 1e30;
	return (delta_dist);
}

typedef struct s_dda_alog
{
	t_vec2 map_pos;
	t_vec2 side_dist;
	t_vec2 step_dir;
	t_vec2 ray;
	int side;
} t_dda_algo;

int _dda(t_game *game, t_vec2 *side_dist, int *mapX, int *mapY, t_vec2 delta_dist, t_vec2 step)
{
	int stop = 0;
	int side;

	while (!stop)
	{
		if (side_dist->x < side_dist->y)
		{
			side_dist->x += (delta_dist.x * TILE_SIZE);
			*mapX += step.x;
			side = 0;
		}
		else
		{
			side_dist->y += (delta_dist.y * TILE_SIZE);
			*mapY += step.y;
			side = 1;
		}
		if (*mapX < 0 || *mapX >= MAP_WIDTH || *mapY < 0 || *mapY >= MAP_HEIGHT)
			stop = 1;
		if ((game->world.map)[*mapY][*mapX])
			stop = 1;
	}
	return (side);
}
void dda(t_game *game)
{
	double camera;
	t_vec2 ray;
	t_player *player;
	t_vec2 delta_dist;
	t_vec2 side_dist;
	t_vec2 step;
	int side;

	player = &game->player;
	for (int camera_x = 0; camera_x < (int)game->screen_width; camera_x++)
	{
		int mapX = player->pos.x / TILE_SIZE;
		int mapY = player->pos.y / TILE_SIZE;
		camera = 2.0 * camera_x / game->screen_width - 1;
		ray = vec2_add(player->dir, vec2_scale(player->plane, camera));
		delta_dist = init_delta_dist(ray);


		if (ray.x < 0)
		{
			side_dist.x = delta_dist.x * (player->pos.x - mapX * TILE_SIZE);
			step.x = -1.0;
		}
		else
		{
			side_dist.x = delta_dist.x * ((mapX + 1) * TILE_SIZE - player->pos.x);
			step.x = 1.0;
		}
		if (ray.y < 0)
		{
			side_dist.y = delta_dist.y * (player->pos.y - mapY * TILE_SIZE);
			step.y = -1.0;
		}
		else
		{
			side_dist.y = delta_dist.y * ((mapY + 1) * TILE_SIZE - player->pos.y);
			step.y = 1.0;
		}
		side = _dda(game, &side_dist, &mapX, &mapY, delta_dist, step);
		double x;
		if(side == 0)
			x = (side_dist.x  / TILE_SIZE - delta_dist.x);
		else
			x = (side_dist.y  / TILE_SIZE - delta_dist.y);
		if ((int)x != 0)
		{
			t_vec2 mini_player_pos = vec2_div(player->pos, MINIMAP_SCALE);
			// printf("Stupid x = %f\n", x);
			draw_line(&game->scene, mini_player_pos, vec2_add(mini_player_pos, vec2_scale(ray, x / MINIMAP_SCALE)), MINDARO);
			double line_height = (int) (game->screen_height / x * TILE_SIZE);
			double h = game->screen_height;
			int drawStart = -line_height / 2 + h / 2;
			if(drawStart < 0)
				drawStart = 0;
			int drawEnd = line_height / 2 + h / 2;
			if(drawEnd >= h)
				drawEnd = h - 1;
			if (side)
			{
				draw_line(&game->scene, vec2_new(camera_x, drawStart), vec2_new(camera_x, drawEnd), PURPLE);
			}
			else
			{
				draw_line(&game->scene, vec2_new(camera_x, drawStart), vec2_new(camera_x, drawEnd), TINY_BLACK);
			}
		}
	}
}

int times[10];

/**
 * average - calculate the average of numbers
 *
 * @stats: array of numbers
 * @size: array size
 *
 * Return: average value
 */
size_t average(int *stats, size_t size)
{
	size_t sum;
	size_t i;

	i = 0;
	sum = 0;
	while (i < size)
	{
		sum += stats[i++];
	}
	return (sum/size);
}

// time stats to be added later:
// static time_t last_frame_time;
// static size_t curr_time;
//
// if (curr_time == 10)
// {
// 	size_t avg = average(times, curr_time);
// 	if (avg > 0)
// 		printf("AVG_FRAME_RATE: %ld\n", 1000/ avg);
// 	curr_time = 0;
// }
// times[curr_time++] = curr_time_ms() - last_frame_time;
// last_frame_time = curr_time_ms();
void player_draw_small(t_game *game)
{
	double scale = TILE_SIZE / MINIMAP_SCALE;
	t_vec2 head;
	t_vec2 mini_player_pos;


	mini_player_pos = vec2_div(game->player.pos, MINIMAP_SCALE);
	head = vec2_add(vec2_scale(game->player.dir, scale), mini_player_pos);
	draw_line(&game->scene, mini_player_pos, head, MINDARO);
	draw_filled_circle(&game->scene, mini_player_pos, 2, WHITE);
	// t_vec2 plane_start = vec2_add(head, vec2_scale(game->player.plane, scale));
	// t_vec2 plane_end = vec2_add(head, vec2_scale(game->player.plane, -scale));
	// draw_line(&game->scene, plane_start, plane_end, PURPLE);
	// draw_filled_square(&game->scene, plane_start, 4, PURPLE);
	// draw_line(&game->scene, game->player.pos, plane_start, PURPLE);
	// draw_line(&game->scene, game->player.pos, plane_end, PURPLE);
	// draw_filled_square(&game->scene, plane_end, 4, PURPLE);
}

int game_loop(t_game *game)
{
	image_clear(&game->scene);
	// draw_filled_square(&game->scene, vec2_new(0, 0), TILE_SIZE, PURPLE);
	// draw_circle(&game->scene, game->mouse_pos, 5, RED);
	// draw_line(&game->scene, game->player.pos, game->mouse_pos, RED);
	// draw_circle(&game->scene, game->player.pos, 5, RED);
	// intersection_points(&game->scene, game->player.pos, game->mouse_pos);
	dda(game);
	draw_grid(&game->scene);
	player_draw_small(game);
	mlx_put_image_to_window(game->mlx, game->win,game->scene.img, 0, 0);
	return (0);
}

int	main()
{
	t_game game;


	init_game(&game);

	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_hook(game.win, 02, 1L<<0, key_even_handler, &game);
	mlx_hook(game.win, 03, 1L<<1, key_even_handler, &game);
	mlx_hook(game.win, 06, 1L<<6, handle_mouse_event, &game); // for mouse
	mlx_loop(game.mlx);
	return (0);
}
