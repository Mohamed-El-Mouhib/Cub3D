#include "../includes/cub3d.h"

void	player_render_frame(t_game *game)
{
	t_animation	*anim;
	t_data		*frame;
	int			x_off;
	int			y_off;

	anim = game->player.animations[game->player.state];
	frame = animation_get_frame(game, anim);
	x_off = game->scene.width - frame->width;
	y_off = game->scene.height - frame->height + 40;
	x_off += game->player.bob.x + game->player.sway;
	y_off += game->player.bob.y;
	image_draw_transparent(game, frame, x_off, y_off);
}
