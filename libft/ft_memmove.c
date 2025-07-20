/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 14:34:28 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/05 16:28:37 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h" 

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*dst;
	unsigned char	*srt;
	size_t			i;

	if (!dest || !src)
		return (NULL);
	dst = (unsigned char *)dest;
	srt = (unsigned char *)src;
	i = 0;
	if (dest < src)
		return (ft_memcpy(dst, srt, n));
	if (dest == NULL && src == NULL)
		return (NULL);
	else
	{
		i = n;
		while (i > 0)
		{
			i--;
			dst[i] = srt[i];
		}
	}
	return (dst);
}
