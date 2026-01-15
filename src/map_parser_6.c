/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser_6.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:49:38 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:49:48 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/map_parse.h"

bool	read_map(t_config *config)
{
	size_t	i;

	if (!check_is_closed(config->map[0]))
		return (log_error("Map surrounded by Invalid character"));
	if (!check_is_closed(config->map[config->map_height - 1]))
		return (log_error("Map surrounded by Invalid character"));
	i = 0;
	while (i < config->map_height)
	{
		if (!check_map_line(config->map, i, config))
			return (false);
		i++;
	}
	if (config->player_x == -1)
		return (log_error("Map has no player"));
	return (true);
}

bool	validate_config(t_config *config)
{
	size_t	i;

	if (!is_config_done(config))
		return (log_error("Missing some required element"));
	i = 0;
	while (i < 4)
	{
		if (!validate_paths(config->value[i]))
			return (false);
		i++;
	}
	if (!parse_color(config->value[TOKEN_C], &config->c))
		return (false);
	if (!parse_color(config->value[TOKEN_F], &config->f))
		return (false);
	return (true);
}

bool	read_file(t_dyn *dyn, t_config *config)
{
	t_lex	lex;
	char	**lines;
	size_t	i;

	ft_bzero(&lex, sizeof(lex));
	if (!read_config(&lex, dyn, config))
		return (false);
	if (!validate_config(config))
		return (false);
	i = lex.map_start;
	lines = (char **)dyn->buff;
	while (i < dyn->length && is_empty_line(lines[i]))
		++i;
	if (i == dyn->length)
		return (log_error("Missing map"));
	config->map = lines + i;
	get_map_size(config, dyn->length - i);
	if (!read_map(config))
		return (false);
	i += config->map_height;
	while (i < dyn->length && is_empty_line(lines[i]))
		++i;
	if (i != dyn->length)
		return (log_error("Map is not the last element"));
	return (true);
}

bool	check_file_name(char *filename)
{
	char	*ptr;

	if (!filename || !*filename)
		return (log_error("No filename provided"));
	ptr = ft_strrchr(filename, '.');
	if (ptr && !ft_strncmp(ptr, ".cub", 5) && ft_strlen(filename) > 4)
		return (true);
	return (log_error("Invalid filename"));
}

/**
 * free_world - Release world memory
 */
void	release_world(t_world *world)
{
	int	i;

	i = 0;
	while (world->map && i < world->map_height)
		free(world->map[i++]);
	free(world->map);
	world->map_width = 0;
	world->map_height = 0;
	world->map = NULL;
}
