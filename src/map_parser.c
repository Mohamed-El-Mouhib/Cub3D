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
	else if (info()->error.err == NO_PLAYER)
		printf(NO_PLAYER_ERR, info()->error.file);
}

bool is_valid_char(char c)
{
	return (ft_strchr(MAP_VALID_CHARACTERS, c));
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

bool	validate_map(t_game* game, t_dyn* dyn, size_t i)
{
	int	(bigst), (j), (len);
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
				game->player.pos.y = i * TILE_SIZE + TILE_SIZE/2;
				game->player.pos.x = j * TILE_SIZE + TILE_SIZE/2;
			}
		}
		i++;
	}
	game->world.map_width = bigst;
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

bool	check_map_line(char** line, size_t len, size_t j)
{
	size_t   i;

	i = 0;
	while (line[j][i])
	{
		if (ft_strchr("0WESNCO", line[j][i]))
		{
			if (!i || line[j][i - 1] == ' ' || !line[j][i + 1]
				|| line[j][i + 1] == ' ' || ft_strlen(line[j - 1]) <= i
				|| line[j - 1][i] == ' ' || j == len - 1
				|| ft_strlen(line[j + 1]) <= i || line[j + 1][i] == ' ')
				return (init_error(UNCLOSED, j, i, &line[j][i]), false);
		}
		else if (!is_valid_char(line[j][i]))
			return (init_error(INVALID_CHAR, j, i, &line[j][i]), false);
		++i;
	}
	return true;
}

typedef struct s_lex
{
	char*	file;
	char*	text;// string text
	size_t  len; // lenght
	size_t	pos; // index
}	t_lex;

bool lex_try_read_str(t_lex *lex, char *str)
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
		return TOKEN_NO;
	else if (lex_try_read_str(lex, "SO "))
		return TOKEN_SO;
	else if (lex_try_read_str(lex, "WE "))
		return TOKEN_WE;
	else if (lex_try_read_str(lex, "EA "))
		return TOKEN_EA;
	else if (lex_try_read_str(lex, "F "))
		return TOKEN_F;
	else if (lex_try_read_str(lex, "C "))
		return TOKEN_C;
	return TOKEN_INVALID;
}


void	lex_init(t_lex* lex, char* str, size_t len, char* file)
{
	lex->text = str;
	lex->len = len;
	lex->pos = 0;
	lex->file = file;
}

void	lex_skip_spaces(t_lex* lex)
{
	while (lex->pos < lex->len)
	{
		if (lex->text[lex->pos] == ' ')
			lex->pos++;
		else
			return;
	}
}

int	try_get_value(t_lex* lex)
{
	int	i;
	char *strr;


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

enum ErrType{
	ERR_DUP_ELEM,
	ERR_MISS_ELEM,
	ERR_UNKNOWN_ELEM,
	ERR_INVALID_VALUE,
	ERR_INVALID_KEY,
} type;

bool	err_print(int type)
{
	printf("Error\n");
	if (type == ERR_DUP_ELEM)
		printf("Duplicated element found\n");
	else if (type == ERR_MISS_ELEM)
		printf("Missing element\n");
	else if (type == ERR_UNKNOWN_ELEM)
		printf("Unknown element found\n");
	else if (type == ERR_INVALID_VALUE)
		printf("Invalid value element found\n");
	else if (type == ERR_INVALID_KEY)
		printf("Invalid key element found\n");
	return (false);
}
char *get_token_type_str(t_tok type)
{
	switch (type)
	{
		case TOKEN_NO: return "TOKEN_NO";
		case TOKEN_SO: return "TOKEN_SO";
		case TOKEN_WE: return "TOKEN_WE";
		case TOKEN_EA: return "TOKEN_EA";
		case TOKEN_F: return "TOKEN_F";
		case TOKEN_C: return "TOKEN_C";
		case TOKEN_INVALID: return "TOKEN_INVALID";
		default:return  "TOKEN_INVALID";
	}
}


bool	config_readline(char* line, t_foo* config, t_lex* lex)
{
	t_tok	type;
	int	val_size;

	lex_skip_spaces(lex);
	type = try_read_type(lex);
	if (type == TOKEN_INVALID)
		return (err_print(ERR_INVALID_KEY));
	lex_skip_spaces(lex);
	if (config->value[type])
		return (err_print(ERR_DUP_ELEM));
	config->value[type] = &lex->text[lex->pos];
	val_size = try_get_value(lex);
	lex_skip_spaces(lex);
	if (!val_size || lex->pos != lex->len)
		return (err_print(ERR_INVALID_VALUE));
	config->value[type][val_size] = '\0';
	return true;
}

void print_config(t_foo config)
{
	for (int i = 0; i < TOKEN_NBR; i++)
	{
		printf("%8s = '%s'\n", get_token_type_str(i), config.value[i]);
	}
}

bool	is_config_done(t_foo* config)
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

bool read_config(t_dyn* dyn, t_foo* config, char* file)
{
	size_t   i;
	t_lex	lex;
	char**   lines;

	i = 0;
	lines = (char**)dyn->buff;
	while (i < dyn->length)
	{
		if (is_config_done(config))
			return true;;
		if (!is_empty_line(lines[i]))
		{

			lex_init(&lex, lines[i], ft_strlen(lines[i]), file);
			if (!config_readline(lines[i], config, &lex))
				return false;
		}
		i++;
	}
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

void	load_content_from_file(int fd, t_dyn* dyn)
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
	t_foo	config;
	int	fd;

	info()->error.file = filename;
	ft_bzero(&config, sizeof(t_config));
	if (!check_file_name(filename))
		return false;
	fd = open_file(filename);
	load_content_from_file(fd, &lines);
	read_config(&lines, &config, filename);
	if (!is_config_done(&config))
		err_print(ERR_MISS_ELEM);
	exit(1);
	// game->world.map = info()->map; //after validating the map, storing it on true map container game.world
	// game->world.map_height = info()->map_height; // setting the true lenght of the map
	// game->floor = info()->f;
	// game->ceiling = info()->c;
	return (true);
}
