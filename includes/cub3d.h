/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:33:13 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/09 19:33:13 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef CUB3D_H
# define CUB3D_H
# include "graphics.h"
# include "dynamic_array.h"

# define TILE_SIZE 80.0
// # define WIN_H (TILE_SIZE * 20 + 1)
// # define WIN_W (TILE_SIZE * 40 + 1)

// minimap
void draw_minimap(t_game *game);
void draw_fov_in_minimap(t_game *game, t_dda_ctx *dda);
void raycast_draw_walls(t_game *game);

// Keyboard events
int	key_press(int key_code, t_game *game);
int	key_release(int key_code, t_game *game);
void    game_handle_keyboard_events(t_game *game);

// player movements
void player_rotate(t_player *player, t_rotate_dir rot_dir);
void player_move(t_player *player, int speed, t_move_dir move_dir);

// Mouse events
int handle_mouse_event(int x,int y, t_game *game);

// Utils
void ft_exit_error(char *msg);
time_t	curr_time_ms(void);
double sign(double x);

// initializers
void init_game(t_game *game, char *filename);

// Parser
bool parse_content(char *filename, t_game* game);
void	init_error(t_error_type type, int line, int index, void* p);

// textures
void	draw_texture_line(t_game *game, t_dda_ctx *info);

bool is_valid_char(char c);

// error
void	error_indexing(void);
#endif
