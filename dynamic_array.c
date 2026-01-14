/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_words.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 17:42:23 by aljbari           #+#    #+#             */
/*   Updated: 2025/07/03 17:42:53 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "includes/dynamic_array.h"

static int	dynamic_ensure_capacity(t_dyn *arr)
{
	size_t	req_cap;
	size_t	new_cap;
	void	**new_buff;

	req_cap = arr->length + 1;
	if (arr->capacity >= req_cap)
		return (0);
	new_cap = arr->capacity * 2;
	new_buff = malloc(new_cap * sizeof(void *));
	if (!new_buff)
	{
		perror("dyn_malloc");
		exit(1);
	}
	ft_memcpy(new_buff, arr->buff, arr->length * sizeof(void *));
	free(arr->buff);
	arr->buff = new_buff;
	arr->capacity = new_cap;
	return (0);
}

void	dyn_add_back(t_dyn *arr, void *elem)
{
	if (arr->capacity == 0)
	{
		printf(ADD_NEW_ELEMENT_FAIL);
		return ;
	}
	dynamic_ensure_capacity(arr);
	arr->buff[arr->length] = elem;
	arr->length++;
}

void dyn_foreach(t_dyn *arr, void f(void *))
{
	size_t i;

	i = 0;
	while (i < arr->length)
		f(arr->buff[i++]);
}

void *dyn_at(t_dyn *arr, size_t index)
{
	// if (index >= arr->length)
	// {
	// 	printf("Error: index %zu out of range (length: %zu)\n", index, arr->length);
	// 	exit(1);
	// }
	return (arr->buff[index]);
}

void dyn_erase(t_dyn *arr, void delete_func(void *))
{
	size_t i;

	i = 0;
	while (i < arr->length)
		delete_func(arr->buff[i++]);
	free(arr->buff);
	arr->length = 0;
	arr->capacity = 0;
	arr->buff = NULL;
}

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
