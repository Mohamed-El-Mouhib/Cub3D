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

t_config*	info(void)
{
	static t_config box;

	return (&box);
}

void	init_error(t_error_type type, int line, int index, void* p)
{
	info()->error.err = type;
	info()->error.cpos = index;
	info()->error.line = line;
	info()->error.ptr = p;
}


#define	FILENAME_ERR "Error\nplease provide filename example: ./path/to/map.cub\n"
#define	PATH_ERR "Error\n'%s' %d:%d - invalid .xpm asset path\n"
#define	IN_FILENAME_ERR "Error\n'%s' must have a .cub extension.\n"
#define	IN_SYNX_ERR "Error\n'%s' %d:%d - syntax error\n"
#define	IN_CHAR_ERR "Error\n'%s' %d:%d - invalid character '%c'\n"
#define	IN_COLOR_ERR "Error\n'%s' %d:%d - invalid color\n"
#define	PLAYER_ERR "Error\n'%s' %d:%d invalid number of player instance\n"
#define	UNFINISHED_ERR "Error\n'%s' is incomplete or missing required settings.\n"
#define	COMMAS_ERR "Error\n'%s': invalid number of commas\n"
#define	CLOSE_ERR "Error\n'%s' %d:%d unclosed wall found (%c)\n"
#define	MAP_ERR "Error\n'%s': map is not found\n"

void	error_indexing(void)
{
	if (info()->error.err == NO_FILENAME)
		printf(FILENAME_ERR);
	else if (info()->error.err== INVALID_PATH)
		printf(PATH_ERR, info()->error.file, info()->error.line, info()->error.cpos);
	else if (info()->error.err== INVALID_COLOR)
		printf(IN_COLOR_ERR, info()->error.file, info()->error.line, info()->error.cpos);
	else if (info()->error.err== INVALID_SYNX)
		printf(IN_SYNX_ERR, info()->error.file, info()->error.line, info()->error.cpos);
	else if (info()->error.err== DOUBLE_P_INSTANCE)
		printf(PLAYER_ERR, info()->error.file, info()->error.line, info()->error.cpos);
	else if (info()->error.err== UNFINISHED)
		printf(UNFINISHED_ERR, info()->error.file);
	else if (info()->error.err== COMMAS)
		printf(COMMAS_ERR, info()->error.file);
	else if (info()->error.err== INVALID_FILENAME)
		printf(IN_FILENAME_ERR, info()->error.file);
	else if (info()->error.err== INVALID_CHAR)
		printf(IN_CHAR_ERR, info()->error.file, info()->error.line, info()->error.cpos, *(char*)info()->error.ptr);
	else if (info()->error.err== UNCLOSED)
		printf(CLOSE_ERR, info()->error.file, info()->error.line, info()->error.cpos, *(char*)info()->error.ptr);
	else if (info()->error.err== NO_MAP)
		printf(MAP_ERR, info()->error.file);
}

bool is_valid_char(char c)
{
	return (ft_strchr(MAP_VALID_CHARACHTERS, c));
}

bool	is_empty_line(char* line)
{
	int	i;

	if (!line || !*line)
		return true;
	i = -1;
	while(line[++i])
	{
		if (line[i] != ' ')
			return false;
	}
	return true;
}

int open_file(char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("Error\n", 2);
		perror(filename);
		exit(1);
	}
	return (fd);
}

t_token	TokenType(char* str)
{
	if (!ft_strncmp(str, "NO ", 3))
		return NO;
	else if (!ft_strncmp(str, "SO ", 3))
		return SO;
	else if (!ft_strncmp(str, "WE ", 3))
		return WE;
	else if (!ft_strncmp(str, "EA ", 3))
		return EA;
	else if (!ft_strncmp(str, "F ", 2))
		return F;
	else if (!ft_strncmp(str, "C ", 2))
		return C;
	return NOT;
}

bool	validate_paths(char* path)
{
	int  i;

	i = -1;
	while(path[++i])
	{
		if (path[i + 1] == ' ' || !path[i + 1])
		{
			if (i > 4 && !ft_strncmp(path + i - 3,".xpm", 4) && is_empty_line(path + i + 1))
			{
				path[i + 1] = '\0';
				return true;
			}
			break;
		}
	}
	init_error(INVALID_PATH, 0, i, path);
	return false;
}

bool	parse_color(char* line, t_color* clr, int* i)
{
	bool has_digit;
	int  value;

	value = 0;
	has_digit = false;
	while (line[*i] == ' ')
		(*i)++;
	while (ft_isdigit(line[*i]))
	{
		has_digit = true;
		value = value * 10 + (line[*i] - '0');
		if (value > 255)
			return (init_error(INVALID_COLOR, 0, info()->error.cpos + *i, NULL)), false;
		(*i)++;
	}
	if (!has_digit)
		return (init_error(INVALID_CHAR, 0, info()->error.cpos + *i, &line[*i])), false;
	*clr = (*clr << 8) | value;
	return (true);
}

void	reform_map_lines(size_t bigest, t_dyn* dyn)
{
	size_t	(i), (len);
	char*   tmp;

	i = 0;
	while (i < info()->map_height)
	{
		len = ft_strlen(info()->map[i]);
		if (len < bigest)
		{
			tmp = malloc(bigest + 1);
			if (!tmp)
				return ((void)perror(""));
			ft_memcpy(tmp, info()->map[i], len);
			ft_memset(tmp + len, ' ', bigest - len);
			tmp[bigest] = '\0';
			info()->map[i] = tmp;
			dyn_add_back(dyn, tmp);
		}
		i++;
	}
}

bool	validate_map(t_game* game, t_dyn* dyn)
{
	int	(bigst), (j), (len);
	size_t i;

	i = 0;
	bigst = 0;
	while (i < info()->map_height)
	{
		j = -1;
		len = ft_strlen(info()->map[i]);
		if (len > bigst)
			bigst = len;
		while (info()->map[i][++j])
		{
			if (ft_strchr("NSEW", info()->map[i][j]))
			{
				if (game->player.pos.x != -1 || game->player.pos.y != -1)
					return init_error(DOUBLE_P_INSTANCE, i, j, NULL), false;
				game->player.pos.y = i * TILE_SIZE;
				game->player.pos.x = j * TILE_SIZE;
			}
		}
		i++;
	}
	return (reform_map_lines(bigst, dyn), true);
}

bool	validate_colors(char *line, t_color* clr)
{
	int  (i), (comma_count);

	i = 0;
	comma_count = 0;
	while (line[i])
	{
		if (!parse_color(line, clr, &i))
			return (false);
		while (line[i] == ' ')
			i++;
		if (line[i] == ',')
		{
			comma_count++;
			if (comma_count > 2)
				return init_error(COMMAS, 0, info()->error.cpos + i, NULL), false;
			i++;
		}
		else if (line[i])
			return init_error(INVALID_CHAR, 0, info()->error.cpos + i, &line[i]), false;
	}
	if (comma_count != 2)
		return init_error(COMMAS, 0, info()->error.cpos + i, NULL), false;
	return true;
}
bool	validate_configs(t_token type)
{
	if (type < 3 && !validate_paths(info()->ptr[type]))
		return false;
	else if (type == F && !validate_colors(info()->ptr[type], &info()->f))
		return false;
	else if (type == C && !validate_colors(info()->ptr[type], &info()->c))
		return false;
	return true;
}

int	TypeStoring(char* str, int* i, t_token type, int *check)
{
	static t_token FirstToken;

	type = TokenType(str + *i);
	if (*check && type <= C)
	{
		FirstToken = type;
		*check = false;
		(*i)++;
		return (-1);
	}
	else if (*check || info()->ptr[FirstToken])
		return (-2);
	info()->ptr[FirstToken] = str + *i;
	return (FirstToken);
}

bool	ParseLine(char* line, size_t* finished)
{
	int	(i), (check), (stat);
	t_token	type;

	check = true;
	i = -1;
	while (line[++i])
	{
		if (line[i] != ' ')
		{
			type = TokenType(line + i);
			stat = TypeStoring(line, &i, type, &check);
			if (stat == -1)
				continue;
			else if (stat == -2)
				break;
			(*finished)++;
			info()->error.cpos = i;
			return (validate_configs(stat));
		}
	}
	init_error(INVALID_SYNX, 0, i, &line[i]);
	return (false);
}

bool	check_is_closed(char* line, int y)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] != '1' && line[i] != ' ')
			 return init_error(UNCLOSED, y, i, &line[i]), false;
	}
	return true;
}

bool	check_map_line(char** line, size_t len, size_t j)
{
	size_t   i;

	i = 0;
	while (line[j][i])
	{
		if (ft_strchr("0WESN", line[j][i]))
		{
			if (!i || line[j][i - 1] == ' ' || !line[j][i + 1]
				|| line[j][i + 1] == ' ' || ft_strlen(line[j - 1]) <= i
				|| line[j - 1][i] == ' ' || j == len - 1
				|| ft_strlen(line[j + 1]) <= i || line[j + 1][i] == ' ')
			{
				printf("line %zu:%zu error: unclosed wall found (%c)\n", j, i, line[j][i]);
				return false;
			}
		}
		else if (!is_valid_char(line[j][i]))
		{
			printf("line %zu:%zu error: invalid char (%c)\n", j, i, line[j][i]);
			return false;
		}
		++i;
	}
	return true;
}

bool	parse_map(t_game* game, t_dyn* dyn, size_t i)
{
	char**	lines;

	lines = (char**)dyn->buff;
	while (i < dyn->length && is_empty_line(lines[i]))
		i++;
	if (i == dyn->length)
		return (init_error(NO_MAP, i, 0, NULL), false);
	if (!check_is_closed(lines[i], i))
		return (false);
	info()->map_height = i;
	info()->map = lines + i;
	while (i < dyn->length)
	{
		if ((i == dyn->length - 1 || is_empty_line(lines[i + 1]))
			&& !check_is_closed(lines[i], i))
			return false;
		else if (!check_map_line(lines, dyn->length, i))
			return false;
		i++;
	}
	info()->map_height = i - info()->map_height;
	return (validate_map(game, dyn));
}

bool extract_elements(t_dyn* dyn)
{
	size_t   (i), (finished);
	char**   lines;

	i = 0;
	finished = 0;
	lines = (char**)dyn->buff;
	while (i < dyn->length && finished < 6)
	{
		if (!*lines[i] || ParseLine(lines[i], &finished))
			i++;
		else
		{
			info()->error.line += i;
			return false;
		}
	}
	info()->map = lines + i;
	info()->map_height = i;
	if (finished < 6)
		return (init_error(UNFINISHED, 0, 0, NULL), false);
	return true;
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

bool	check_file_name(char *filename)
{
	if (!filename)
		info()->error.err = NO_FILENAME;
	else if (!*filename)
		info()->error.err = EMPTY_FILE;
	else if (!ft_strncmp(ft_strrchr(filename, '.'), ".cub", 5) && ft_strlen(filename) > 4)
	{
		info()->error.err = NO_ERR;
		return true;
	}
	else
		info()->error.err = INVALID_FILENAME;
	return false;
}

void	load_content_from_map(int fd, t_dyn* dyn)
{
	char* line;

	*dyn = dyn_init();
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break;
		delete_last_newline(line);
		dyn_add_back(dyn, line);
	}
	close(fd);
}

bool parse_content(char *filename, t_game* game)
{
	t_dyn	lines;
	int	fd;

	info()->error.file = filename;
	if (!check_file_name(filename))
		return false;
	fd = open_file(filename);
	load_content_from_map(fd, &lines);
	if (!extract_elements(&lines))
		return (false);
	if (!parse_map(game, &lines, info()->map_height))
		return (false);
	game->world.map = info()->map; //after validating the map, storing it on true map container game.world
	game->world.map_height = info()->map_height; // setting the true lenght of the map
	game->world.map_width = info()->map_width; // afte the reform of the map everything is
	game->floor = info()->f;
	game->ceiling = info()->c;
	return (true);
}
