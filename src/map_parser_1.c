/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser_1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:46:06 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:46:09 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/map_parse.h"

bool	log_error(char *error)
{
	printf("Error\n%s\n", error);
	return (false);
}

void	release_file_exit(t_dyn *file, t_game *game)
{
	int	i;

	dyn_erase(file, free);
	i = 0;
	while (i < 4)
	{
		free(game->world.values[i]);
		i++;
	}
	release_map(game);
	exit(1);
}

bool	is_valid_char(char c)
{
	return (ft_strchr(MAP_VALID_CHARACTERS, c));
}

bool	is_empty_line(char *line)
{
	int	i;

	if (!line || !*line)
		return (true);
	i = -1;
	while (line[++i])
	{
		if (line[i] != ' ')
			return (false);
	}
	return (true);
}

int	open_file(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error\n", 2);
		perror(filename);
		exit(1);
	}
	return (fd);
}
