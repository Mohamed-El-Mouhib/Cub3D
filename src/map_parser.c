/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:43:14 by aljbari           #+#    #+#             */
/*   Updated: 2025/08/17 16:43:14 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"
#define MAP_VALID_CHARACHTERS "01NSEW "

int open_file(char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror(filename);
		exit(1);
	}
	return (fd);
}

void validate_map_or_exit(t_dyn *lines)
{
	size_t i;
	size_t len;

	if (lines->length == 0)
	{
		dyn_erase(lines, free);
		ft_exit_error("Empty map\n");
	}
	i = 0;
	len = ft_strlen(lines->buff[0]);
	while (i < lines->length)
	{
		if (len != ft_strlen(lines->buff[i]))
		{
			dyn_erase(lines, free);
			ft_exit_error("Map should have the same line length!\n");
		}
		i++;
	}
}

bool is_valid_char(char c)
{
	return (ft_strchr(MAP_VALID_CHARACHTERS, c));
}

void validate_chars_or_exit(t_dyn *lines, char *file)
{
	char *line;
	size_t i;

	i = 0;
	while (i < lines->length)
	{
		line = dyn_at(lines, i);
		while (*line)
		{
			if (!is_valid_char(*line))
			{
				printf("%s:%zu: error: unknown charachter found (%c)\n", file, i, *line);
				dyn_erase(lines, free);
				exit(1);
			}
			line++;
		}
		i++;
	}
}

void delete_last_newline(char *line)
{
	size_t i;

	i = 0;
	while (line && line[i])
	{
		if (line[i] == '\n' && !line[i + 1])
			line[i] = '\0';
		i++;
	}
}

t_dyn read_map_from_file(char *filename)
{
	t_dyn lines;
	char *line;
	int fd;

	fd = open_file(filename);
	lines = dyn_init();
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		delete_last_newline(line);
		dyn_add_back(&lines, line);
	}
	close(fd);
	validate_map_or_exit(&lines);
	validate_chars_or_exit(&lines, filename);
	return (lines);
}

