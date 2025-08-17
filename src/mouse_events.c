/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_events.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:02:38 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 11:02:38 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int handle_mouse_event(int x,int y, t_game *game)
{
	game->mouse_pos.x = x;
	game->mouse_pos.y = y;
	return (0);
}

