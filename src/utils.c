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

void ft_exit_error(char *msg)
{
	write(STDERR_FILENO, msg, ft_strlen(msg));	
	exit(1);
}

time_t	curr_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

double sign(double x)
{
	return ((x > 0) - (x < 0));
}
