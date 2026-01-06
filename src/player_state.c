#include "../includes/cub3d.h"

void player_reload_state(t_game *game)
{
	t_animation *anim;
	t_player *p;

	p = &game->player;
	anim = game->player.animations[PLAYER_RELOAD];
	if (p->ammo == PLAYER_MAX_AMMO)
	{
		p->state = PLAYER_WALKING;
	}
	if (anim->finished)
	{
		if (p->ammo < PLAYER_MAX_AMMO)
			p->ammo = PLAYER_MAX_AMMO;
		p->state = PLAYER_WALKING;
		anim->finished = false;
	}
}

void player_shooting_state(t_game *game)
{
	t_animation *anim;
	t_player *p;

	p = &game->player;
	anim = game->player.animations[PLAYER_SHOOTING];
	if (p->ammo <= 0)
	{
		p->state = PLAYER_WALKING;
	}
	if (anim->finished)
	{
		if (p->ammo > 0)
			p->ammo--;
		if (!game->keyboard_events[KEY_SPACE])
			p->state = PLAYER_WALKING;
		anim->finished = false;
	}
}

void player_update_state(t_game *game)
{
	if (game->player.state == PLAYER_WALKING)
		return ;
	else if (game->player.state == PLAYER_RELOAD)
		player_reload_state(game);
	else if (game->player.state == PLAYER_SHOOTING)
		player_shooting_state(game);
}
