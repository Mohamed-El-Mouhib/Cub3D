
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
	{1, 1, 0, 0, 0, 1, 0, 0, 0, 1},
	{1, 1, 0, 0, 0, 1, 0, 0, 0, 1},
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
	ROTATE_LEFT  = -1,
	ROTATE_RIGHT = 1
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

void draw_grid(t_data  *buff)
{
	t_vec2 start;
	int i;

	start = vec2_new(0, 0);
	i = 0;
	while (i <= buff->width)
	{
		start.y = i;
		draw_horizontal_line(buff, start, buff->width, RED);
		i+= TILE_SIZE;
	}
	start = vec2_new(0, 0);
	i = 0;
	while (i <= buff->width)
	{
		start.x = i;
		draw_vertical_line(buff, start, buff->height, RED);
		i+= TILE_SIZE;
	}
	i = 0;
	while (i < MAP_HEIGHT)
	{
		int j = 0;
		while (j < MAP_WIDTH)
		{
			if (map[i][j])
				draw_filled_square(buff, vec2_new(j * TILE_SIZE,i * TILE_SIZE), TILE_SIZE, TINY_BLACK);
			j++;
		}
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

t_vec2 init_delta_step(t_vec2 ray)
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

void dda(t_game *game)
{
	double camera;
	t_vec2 ray;
	double ray_step;
	t_player *player;
	t_vec2 delta_dist;
	t_vec2 side_dist;
	t_vec2 step;
	int side;

	player = &game->player;
	ray_step = 2.0 / game->screen_width;
	camera = -1;
	while (camera <= 1)
	{
		int mapX = player->pos.x / TILE_SIZE;
		int mapY = player->pos.y / TILE_SIZE;
		// draw_line(&game->scene, player->pos, vec2_add(vec2_scale(game->player.dir, 300), game->player.pos), RED);
		ray = vec2_add(player->dir, vec2_scale(player->plane, camera));
		delta_dist = init_delta_step(ray);


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
		// vec2_print(cord_step, "CORD_STEP");
		// draw_circle(&game->scene, vec2_scale(vec2_new(mapX, mapY), TILE_SIZE), 4, PURPLE);
		int stop = 0;
		while (!stop)
		{
			if (side_dist.x < side_dist.y)
			{
				side_dist.x += (delta_dist.x * TILE_SIZE);
				mapX += step.x;
				side = 0;
			}
			else
			{
				side_dist.y += (delta_dist.y * TILE_SIZE);
				mapY += step.y;
				side = 1;
			}
			if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
				stop = 1;
			if ((game->world.map)[mapY][mapX])
				break ;
		}
		double x;
		if(side == 0) x = (side_dist.x  / TILE_SIZE- delta_dist.x);
		else          x = (side_dist.y  / TILE_SIZE- delta_dist.y);
		draw_line(&game->scene, player->pos, vec2_add(player->pos, vec2_scale(ray, x)), PURPLE);
		camera += ray_step;
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
void draw_player(t_game *game)
{
	double scale = TILE_SIZE / 2;
	t_vec2 head;

	head = vec2_add(vec2_scale(game->player.dir, scale), game->player.pos);
	// draw_line(&game->scene, game->player.pos, head, PURPLE);
	draw_filled_circle(&game->scene, game->player.pos, 6, RED);
	t_vec2 plane_start = vec2_add(head, vec2_scale(game->player.plane, scale));
	t_vec2 plane_end = vec2_add(head, vec2_scale(game->player.plane, -scale));
	draw_line(&game->scene, plane_start, plane_end, PURPLE);
	draw_filled_square(&game->scene, plane_start, 4, PURPLE);
	draw_line(&game->scene, game->player.pos, plane_start, PURPLE);
	draw_line(&game->scene, game->player.pos, plane_end, PURPLE);
	draw_filled_square(&game->scene, plane_end, 4, PURPLE);
}

int game_loop(t_game *game)
{
	image_clear(&game->scene);
	draw_grid(&game->scene);
	// draw_filled_square(&game->scene, vec2_new(0, 0), TILE_SIZE, PURPLE);
	// draw_circle(&game->scene, game->mouse_pos, 5, RED);
	// draw_line(&game->scene, game->player.pos, game->mouse_pos, RED);
	// draw_circle(&game->scene, game->player.pos, 5, RED);
	// intersection_points(&game->scene, game->player.pos, game->mouse_pos);
	draw_player(game);
	dda(game);
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
