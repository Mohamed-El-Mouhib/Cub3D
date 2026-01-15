/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 11:03:56 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 11:03:56 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/cub3d.h"

time_t	curr_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

double	sign(double x)
{
	return ((x > 0) - (x < 0));
}

double	lerp(double start, double end, double t)
{
	return (start + (end - start) * t * (t < 1));
}

t_vec2	vec2_lerp(t_vec2 start, t_vec2 end, double factor)
{
	t_vec2	res;

	res.x = lerp(start.x, end.x, factor);
	res.y = lerp(start.y, end.y, factor);
	return (res);
}
