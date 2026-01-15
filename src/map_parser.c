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
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#define MAP_VALID_CHARACTERS "01NSEWOC "

typedef struct s_lex
{
	char	*file;
	char *text;       // string text
	size_t len;       // length
	size_t pos;       // index
	size_t map_start; // index
}			t_lex;

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

bool	check_is_closed(char *line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] != '1' && line[i] != ' ')
			return (false);
	}
	return (true);
}

bool	lex_try_read_str(t_lex *lex, char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (lex->pos + len > lex->len)
		return (false);
	if (!ft_strncmp(&lex->text[lex->pos], str, len))
	{
		lex->pos += len;
		return (true);
	}
	else
		return (false);
}

t_tok	try_read_type(t_lex *lex)
{
	if (lex_try_read_str(lex, "NO "))
		return (TOKEN_NO);
	else if (lex_try_read_str(lex, "SO "))
		return (TOKEN_SO);
	else if (lex_try_read_str(lex, "WE "))
		return (TOKEN_WE);
	else if (lex_try_read_str(lex, "EA "))
		return (TOKEN_EA);
	else if (lex_try_read_str(lex, "F "))
		return (TOKEN_F);
	else if (lex_try_read_str(lex, "C "))
		return (TOKEN_C);
	return (TOKEN_INVALID);
}

void	lex_init(t_lex *lex, char *str, size_t len)
{
	lex->text = str;
	lex->len = len;
	lex->pos = 0;
}

void	lex_skip_spaces(t_lex *lex)
{
	while (lex->pos < lex->len)
	{
		if (lex->text[lex->pos] == ' ')
			lex->pos++;
		else
			return ;
	}
}

int	try_get_value(t_lex *lex)
{
	int	i;

	i = 0;
	while (lex->pos < lex->len)
	{
		if (lex->text[lex->pos] == ' ')
			return (i);
		i++;
		lex->pos++;
	}
	return (i);
}

char	*get_token_type_str(t_tok type)
{
	switch (type)
	{
	case TOKEN_NO:
		return ("TOKEN_NO");
	case TOKEN_SO:
		return ("TOKEN_SO");
	case TOKEN_WE:
		return ("TOKEN_WE");
	case TOKEN_EA:
		return ("TOKEN_EA");
	case TOKEN_F:
		return "TOKEN_F";
	case TOKEN_C:
		return "TOKEN_C";
	case TOKEN_INVALID:
		return "TOKEN_INVALID";
	default:
		return "TOKEN_INVALID";
	}
}

bool	config_readline(t_config *config, t_lex *lex)
{
	t_tok	type;
	int		val_size;

	lex_skip_spaces(lex);
	type = try_read_type(lex);
	if (type == TOKEN_INVALID)
		return (log_error("Invalid or missing key element"));
	lex_skip_spaces(lex);
	if (config->value[type])
		return (log_error("Duplicated element found"));
	config->value[type] = &lex->text[lex->pos];
	val_size = try_get_value(lex);
	lex_skip_spaces(lex);
	if (!val_size)
		return (log_error("Invalid value element found"));
	return true;
}

void	print_config(t_config *config)
{
	int	i;

	i = 0;
	while (i < TOKEN_NBR)
	{
		printf("%8s = '%s'\n", get_token_type_str(i), config->value[i]);
		i++;
	}
}

bool	is_config_done(t_config *config)
{
	int	i;

	i = -1;
	while (++i < TOKEN_NBR)
	{
		if (!config->value[i])
			return false;
	}
	return true;
}

bool	read_config(t_lex *lex, t_dyn *dyn, t_config *config)
{
	size_t	i;
	char	**lines;

	i = 0;
	lines = (char **)dyn->buff;
	while (i < dyn->length)
	{
		if (!is_empty_line(lines[i]))
		{
			lex_init(lex, lines[i], ft_strlen(lines[i]));
			if (!config_readline(config, lex))
				return false;
		}
		else if (is_config_done(config))
			break ;
		i++;
	}
	lex->map_start = i;
	return true;
}

void	delete_last_newline(char *line)
{
	size_t	i;

	i = 0;
	while (line && line[i])
	{
		if (line[i] == '\n' && !line[i + 1])
			line[i] = '\0';
		i++;
	}
}

void	load_content_from_file(int fd, t_dyn *dyn)
{
	char	*line;

	*dyn = dyn_init();
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		delete_last_newline(line);
		dyn_add_back(dyn, line);
	}
	close(fd);
}

int	lex_color_octet(t_lex *lex)
{
	bool	has_digit;
	int		value;

	value = 0;
	has_digit = false;
	while (lex->pos < lex->len && ft_isdigit(lex->text[lex->pos]))
	{
		has_digit = true;
		value = value * 10 + (lex->text[lex->pos] - '0');
		if (value > 255)
			return (-1);
		lex->pos++;
	}
	if (!has_digit)
		return (-1);
	return (value);
}

bool	parse_last_octet(t_lex *lex, t_color *c)
{
	int	octet;

	octet = lex_color_octet(lex);
	if (octet == -1)
		return (log_error("Invalid octet"));
	*c = (*c << 8) | octet;
	lex_skip_spaces(lex);
	if (lex->pos != lex->len)
		return (log_error("Invalid octet"));
	return (true);
}

bool	parse_color(char *color, t_color *c)
{
	int		i;
	int		octet;
	t_lex	lex;

	*c = 0;
	i = 0;
	lex_init(&lex, color, ft_strlen(color));
	while (i < 2)
	{
		octet = lex_color_octet(&lex);
		if (octet == -1)
			return (log_error("Invalid octet"));
		lex_skip_spaces(&lex);
		if (!lex_try_read_str(&lex, ","))
			return (log_error("Invalid color format\n"));
		lex_skip_spaces(&lex);
		*c = (*c << 8) | octet;
		++i;
	}
	return (parse_last_octet(&lex, c));
}

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

void	get_map_size(t_config *config, size_t end)
{
	size_t	i;
	size_t	width;
	size_t	len;

	i = 0;
	width = 0;
	while (i < end && !is_empty_line(config->map[i]))
	{
		len = ft_strlen(config->map[i]);
		if (len > width)
			width = len;
		i++;
	}
	config->map_height = i;
	config->map_width = width;
}

/**
 * is_val - Check if the charachter is in the right position
 *
 * For example we can't have W (player) next to ' ' (void space)
 */
bool	is_val(char *curr, char *next, char *prev, size_t i)
{
	if (i == 0)
		return (false);
	if (curr[i - 1] == ' ' || !curr[i + 1] || curr[i + 1] == ' ')
		return (false);
	if (ft_strlen(prev) <= i || prev[i] == ' ')
		return (false);
	if (ft_strlen(next) <= i || next[i] == ' ')
		return (false);
	return (true);
}

static bool	set_player(int x, int y, char dir, t_config *config)
{
	if (config->player_x != -1)
		return (log_error("Duplicated Player instance"));
	config->player_x = x;
	config->player_y = y;
	config->player_dir = dir;
	return (true);
}

bool	check_map_line(char **lines, size_t j, t_config *config)
{
	size_t	i;
	char	c;

	i = 0;
	while (lines[j][i])
	{
		c = lines[j][i];
		if (ft_strchr("0WESNCO", c))
		{
			if (ft_strchr("WESN", c) && !set_player(i, j, c, config))
				return (false);
			if (!is_val(lines[j], lines[j + 1], lines[j - 1], i))
				return (log_error("Invalid character position"));
		}
		else if (!is_valid_char(lines[j][i]))
			return (log_error("Unknown character"));
		++i;
	}
	return true;
}

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
	return true;
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
	return true;
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
