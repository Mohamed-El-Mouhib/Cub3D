/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 20:43:45 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/05 16:28:37 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h" 

static int	leen(int n)
{
	int	i;
	int	m;

	i = 0;
	m = n;
	while (m > 0)
	{
		m /= 10;
		i++;
	}
	return (i);
}

static char	*filler(int n, int isnegative)
{
	int		i;
	char	*str;

	i = leen(n) + isnegative;
	str = (char *)malloc((i + 1) * sizeof(char));
	if (str == NULL)
		return (NULL);
	str[i--] = '\0';
	while (n > 0)
	{
		str[i] = (n % 10) + '0';
		n /= 10;
		i--;
	}
	if (isnegative == 1)
		str[i] = '-';
	return (str);
}

char	*ft_itoa(int n)
{
	int	isnegative;

	isnegative = 0;
	if (n == 0)
		return (ft_strdup("0"));
	else if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	else if (n < 0)
	{
		isnegative = 1;
		n = -n;
	}
	return (filler(n, isnegative));
}
