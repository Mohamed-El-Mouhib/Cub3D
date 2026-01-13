#include "../includes/cub3d.h"


t_data *assets_get(t_game *game, int id)
{
	return (dyn_at(game->assets, id));
}

t_data *assets_get_data_from_path(t_game *game, char *path)
{
	int id;

	id = assets_load_xpm(game, path);
	return (assets_get(game, id));
}

/**
 * assets_load_xpm - Loading an exp
 */
int assets_load_xpm(t_game *game, char *path)
{
	t_data *data;

	data = malloc(sizeof(t_data));
	if (!image_load_xpm_or_exit(game, data, path))
		return (-1);
	dyn_add_back(game->assets, data); 
	return (game->assets->length - 1);
}

t_animation *init_animation(size_t start, size_t end, int duration)
{
	t_animation *anim;

	anim = malloc(sizeof(t_animation));
	anim->start = start;
	anim->end = end;
	anim->curr = start;
	anim->duration = duration;
	anim->last_changed = curr_time_ms();
	anim->finished = false;
	return (anim);
}

/**
 * Load @size of frames using @paths, the duration is the time between
 * each frame when cycling through animation frames
 */
t_animation *load_animation_frames(t_game *game, char **paths, size_t size, int duration)
{
	t_animation *	anim;
	size_t		i;
	int		start;
	int		end;

	start = assets_load_xpm(game, paths[0]);
	end = start;
	i = 1;
	while (i < size && size > 1)
	{
		end = assets_load_xpm(game, paths[i]);
		i++;
	}
	anim = init_animation(start, end, duration);
	return (anim);
}
