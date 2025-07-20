/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 16:55:08 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/05 16:28:37 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h" 

size_t	ft_strlcat(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	j;
	size_t	slen;
	size_t	dlen;

	i = 0;
	slen = ft_strlen(src);
	if (!src)
		return (0);
	if (size == 0 && dest == NULL)
		return (slen);
	j = ft_strlen(dest);
	dlen = ft_strlen(dest);
	if (size <= dlen)
		return (size + slen);
	while (src[i] != '\0' && i < size - dlen - 1)
		dest[j++] = src[i++];
	dest[j] = '\0';
	return (dlen + slen);
}
