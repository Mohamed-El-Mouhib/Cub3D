/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_array_1.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 16:35:24 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 16:35:24 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/dynamic_array.h"

t_dyn	dyn_init(void)
{
	t_dyn	dyn;

	dyn.length = 0;
	dyn.capacity = INIT_DYN_ARRAY_SIZE;
	dyn.buff = malloc(dyn.capacity * sizeof(void *));
	return (dyn);
}

t_dyn	*dyn_init_ptr(void)
{
	t_dyn	*dyn;

	dyn = malloc(sizeof(t_dyn));
	*dyn = dyn_init();
	return (dyn);
}
