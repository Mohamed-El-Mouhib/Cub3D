/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dynamic_array.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 12:34:59 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 12:34:59 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

#define ADD_NEW_ELEMENT_FAIL "Warning: attempting to \
	add new element to empty dyncamic array, call dyn init first\n"

#define INIT_DYN_ARRAY_SIZE 16

typedef struct s_dyn
{
	size_t	capacity;
	size_t	length;
	size_t	unit_size;
	void	**buff;
}			t_dyn;

t_dyn		dyn_init(void);
t_dyn		*dyn_init_ptr(void);
void		dyn_erase(t_dyn *arr, void delete_func(void *));
void		dyn_add_back(t_dyn *arr, void *elem);
void		dyn_foreach(t_dyn *arr, void f(void *));
void		*dyn_at(t_dyn *arr, size_t index);
