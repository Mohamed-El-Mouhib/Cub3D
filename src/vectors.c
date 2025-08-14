/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vectors.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 19:27:29 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/09 19:27:29 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../includes/graphics.h"

t_vec2 vec2_new(double x, double y)
{
	t_vec2 vector;

	vector.x = x;
	vector.y = y;
	return (vector);
}

/**
 * vec2_sub - performe subtraction of two vectors
 *
 * @p1: first vectore
 * @p2: second vectore
 *
 * Return: New vectore containing (x,y) result of subtraction
 */
t_vec2 vec2_sub(t_vec2 p1, t_vec2 p2)
{
	t_vec2 result;

	result.x = p1.x - p2.x;
	result.y = p1.y - p2.y;
	return (result);
}


/**
 * vec2_len_squared - Calculate squared distance between two 2D points
 *
 * @p1: First 2D point
 * @p2: Second 2D point
 *
 * Return: Squared distance between p1 and p2
 */
double vec2_len_squared(t_vec2 p1, t_vec2 p2)
{
	return ((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}

/**
 * vec2_len - Calculate distance between two 2D points
 *
 * @p1: First 2D point
 * @p2: Second 2D point
 *
 * Return: Distance (or magnitude) between p1 and p2
 */
double vec2_len(t_vec2 p1, t_vec2 p2)
{
	return (sqrt(vec2_len_squared(p1, p2)));
}

/**
 * vec2_print - print 2d vector
 *
 * @v: Your salty vectore that causing problems
 * @prefix: text will be placed in front of the vectore
 */
void vec2_print(t_vec2 v, char *prefix)
{
	printf("%s: Vec(%f, %f)\n", prefix, v.x, v.y);
}

/**
 */
t_vec2 vec2_add(t_vec2 v1, t_vec2 v2)
{
	return (vec2_new(v1.x + v2.x, v1.y + v2.y));
}

t_vec2 vec2_scale(t_vec2 v1, double factor)
{
	return (vec2_new(v1.x * factor, v1.y * factor));
}

t_vec2 vec2_div(t_vec2 v1, double factor)
{
	return (vec2_new(v1.x / factor, v1.y / factor));
}
