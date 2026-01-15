/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 17:28:28 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 18:04:27 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/cub3d.h"
#include "includes/graphics.h"
#include "includes/types.h"

int	game_loop(t_game *game)
{
	game_handle_inputs(game);
	game_update(game);
	game_render(game);
	render_message(game);
	return (0);
}

int	destroy(t_game *game)
{
	release_game_and_exit(game, EXIT_SUCCESS);
	return (0);
}

int	main(int ac, char **av)
{
	t_game	game;

	if (ac != 2)
		return (log_error("No map provided"));
	init_game(&game, av[1]);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_hook(game.win, 6, 1L << 6, handle_mouse_move, &game);
	mlx_hook(game.win, 2, 1L << 0, key_press, &game);
	mlx_hook(game.win, 3, 1L << 1, key_release, &game);
	mlx_hook(game.win, 4, 1L << 2, handle_mouse_press, &game);
	mlx_hook(game.win, 5, 1L << 3, handle_mouse_release, &game);
	mlx_hook(game.win, 17, 0, destroy, &game);
	mlx_loop(game.mlx);
	return (0);
}
