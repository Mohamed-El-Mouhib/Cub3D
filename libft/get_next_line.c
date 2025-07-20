/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-mouh <mel-mouh@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/23 14:52:15 by mel-mouh          #+#    #+#             */
/*   Updated: 2025/07/05 16:50:02 by mel-mouh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/libft.h" 

char	*ft_gnl_memcpy(char *dest, char *src, size_t n)
{
	size_t			i;

	i = 0;
	while (i < n)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

size_t	ft_strch(const char *str, int c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == (char)c)
			return (i);
		i++;
	}
	return (0);
}

char	*ft_gnl_strjoin(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*str;

	len1 = 0;
	len2 = 0;
	if (s1)
		len1 = ft_strlen(s1);
	if (s2)
		len2 = ft_strlen(s2);
	str = malloc(len1 + len2 + 1);
	if (str == NULL)
		return (NULL);
	ft_gnl_memcpy(str, s1, len1);
	ft_gnl_memcpy(str + len1, s2, len2);
	str[len1 + len2] = '\0';
	free(s1);
	return (str);
}

static char	*extract_line(char **remainder, char *buffer)
{
	char	*line;
	char	*newline_pos;
	char	*new_remainder;

	newline_pos = ft_strchr(*remainder, '\n');
	if (buffer)
		free(buffer);
	if (newline_pos)
	{
		line = malloc((ft_strch(*remainder, '\n') + 2) * sizeof(char));
		ft_gnl_memcpy(line, *remainder, ft_strch(*remainder, '\n') + 1);
		new_remainder = ft_gnl_strjoin(NULL, newline_pos + 1);
		free(*remainder);
		*remainder = new_remainder;
	}
	else
	{
		line = ft_gnl_strjoin(NULL, *remainder);
		free(*remainder);
		*remainder = NULL;
	}
	return (line);
}

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*buffer;
	ssize_t		bytes_read;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	buffer = malloc(((size_t)BUFFER_SIZE + 1) * sizeof(char));
	if (buffer == NULL)
		return (NULL);
	bytes_read = read(fd, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		remainder = ft_gnl_strjoin(remainder, buffer);
		if (ft_strchr(remainder, '\n'))
			return (extract_line(&remainder, buffer));
		bytes_read = read(fd, buffer, BUFFER_SIZE);
	}
	free(buffer);
	if (remainder && *remainder)
		return (extract_line(&remainder, NULL));
	free(remainder);
	remainder = NULL;
	return (NULL);
}
