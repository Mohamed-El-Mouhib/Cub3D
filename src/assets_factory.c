#include "../includes/cub3d.h"
#include <stddef.h>


t_data *assets_get(t_game *game, int id)
{
	return (dyn_at(game->assets, id));
}

/**
 * assets_load_xpm - Loading an exp
 */
int assets_load_xpm(t_game *game, char *path)
{
	t_data *data;

	data = malloc(sizeof(t_data));
	if (!image_load_xpm(game, data, path))
	{
		printf("Error: %s: Frame loading failed: Exiting...\n", __func__);
		printf("* Delete this message when you handle releasing resources *\n");
		exit(1);
		return (-1);
	}
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

t_animation *load_animation_frames(t_game *game, char **paths, size_t size)
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
	anim = init_animation(start, end, 190);
	return (anim);
}
