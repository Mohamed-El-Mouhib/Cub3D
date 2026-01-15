/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:43:14 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:52:00 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/map_parse.h"

#define MAP_VALID_CHARACTERS "01NSEWOC "

bool	validate_paths(char *path)
{
	int	i;

	i = -1;
	while (path[++i])
	{
		if (path[i + 1] != ' ' && path[i + 1])
			continue ;
		if (i >= 4 && !ft_strncmp(path + i - 3, ".xpm", 4) && is_empty_line(path
				+ i + 1))
		{
			path[i + 1] = '\0';
			return (true);
		}
		break ;
	}
	return (log_error("Invalid path"));
}

t_world	build_map(t_config *config)
{
	t_world	world;
	int		i;
	size_t	len;

	ft_bzero(&world, sizeof(world));
	world.map_width = config->map_width;
	world.map_height = config->map_height;
	i = 0;
	world.map = ft_calloc(config->map_height, sizeof(char *));
	if (!world.map)
		release_world(&world);
	while (i < world.map_height)
	{
		len = ft_strlen(config->map[i]);
		world.map[i] = ft_calloc(world.map_width + 1, sizeof(char));
		if (!world.map[i])
			release_world(&world);
		ft_memcpy(world.map[i], config->map[i], len);
		ft_memset(world.map[i] + len, ' ', world.map_width - len);
		i++;
	}
	return (world);
}

bool	construct_game(t_config *config, t_game *game)
{
	t_vec2	pos;

	pos = vec2_new(config->player_x * TILE_SIZE + TILE_SIZE / 2,
			config->player_y * TILE_SIZE + TILE_SIZE / 2);
	game->ceiling = config->c;
	game->floor = config->f;
	game->player.pos = pos;
	game->world = build_map(config);
	game->world.values[WALL_NORTH] = ft_strdup(config->value[TOKEN_NO]);
	game->world.values[WALL_SOUTH] = ft_strdup(config->value[TOKEN_SO]);
	game->world.values[WALL_WEST] = ft_strdup(config->value[TOKEN_WE]);
	game->world.values[WALL_EAST] = ft_strdup(config->value[TOKEN_EA]);
	return (true);
}

bool	parse_content(char *filename, t_game *game)
{
	t_config	config;
	t_dyn		lines;
	int			fd;

	ft_bzero(&config, sizeof(config));
	config.player_x = -1;
	config.player_y = -1;
	if (!check_file_name(filename))
		exit(1);
	fd = open_file(filename);
	load_content_from_file(fd, &lines);
	if (!read_file(&lines, &config))
		release_file_exit(&lines, game);
	if (!construct_game(&config, game))
		release_file_exit(&lines, game);
	dyn_erase(&lines, free);
	return (true);
}
