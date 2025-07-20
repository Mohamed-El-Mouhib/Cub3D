/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/27 18:41:45 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/05 16:28:37 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h" 

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	char	*s2;

	if (!s1)
		return (NULL);
	i = 0;
	j = ft_strlen(s1);
	if (s1 == NULL || set == NULL)
		return (NULL);
	while (s1[i] != '\0' && ft_strchr(set, s1[i]) != NULL)
		i++;
	while (j > i && ft_strchr(set, s1[j - 1]) != NULL)
		j--;
	s2 = malloc(j - i + 1);
	if (s2 == NULL)
		return (NULL);
	ft_memcpy(s2, s1 + i, j - i);
	s2[j - i] = '\0';
	return (s2);
}
