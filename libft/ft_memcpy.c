/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/0/26 13:20:53 by mel-mouh          #+#    #+#             */
/*   Updated: 2024/11/08 11:40:16 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h" 

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t			i;
	unsigned char	*dst;
	unsigned char	*str;

	if (!dest || !src)
		return (NULL);
	i = 0;
	dst = (unsigned char *)dest;
	str = (unsigned char *)src;
	if (dest == src)
		return (dest);
	while (i < n)
	{
		dst[i] = str[i];
		i++;
	}
	return (dst);
}
