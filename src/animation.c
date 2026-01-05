#include "../includes/cub3d.h"

void animation_cycle(t_game *game, t_animation *anim)
{
	if (!anim->is_running || (size_t)(game->tick - anim->last_changed) < anim->duration)
		return ;
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
}

t_data *animation_get_frame(t_game *game, t_animation *anim)
{
	return (dyn_at(game->assets, anim->curr));
}
