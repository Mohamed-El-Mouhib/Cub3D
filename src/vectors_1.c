/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:30:51 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 16:30:51 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/graphics.h"

/**
 * vec2_print - print 2d vector
 *
 * @v: Your salty vectore that causing problems
 * @prefix: text will be placed in front of the vectore
 */
void	vec2_print(t_vec2 v, char *prefix)
{
	printf("%s: Vec(%.2f, %.2f)\n", prefix, v.x, v.y);
}

/**
 */
t_vec2	vec2_add(t_vec2 v1, t_vec2 v2)
{
	return (vec2_new(v1.x + v2.x, v1.y + v2.y));
}

t_vec2	vec2_scale(t_vec2 v1, double factor)
{
	return (vec2_new(v1.x * factor, v1.y * factor));
}

t_vec2	vec2_div(t_vec2 v1, double factor)
{
	return (vec2_new(v1.x / factor, v1.y / factor));
}

t_vec2	vec2_unit(t_vec2 from, t_vec2 to)
{
	return (vec2_div(vec2_sub(to, from), vec2_len(from, to)));
}
