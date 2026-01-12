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
	char*	file;
	char*	text;// string text
	size_t  len; // lenght
	size_t	pos; // index
	size_t	map_start; // index
}	t_lex;

enum ErrType{
	ERR_DUP_ELEM,
	ERR_UNKNOWN_ELEM,
	ERR_INVALID_VALUE,
	ERR_INVALID_PATH,
	ERR_INVALID_KEY,
	ERR_INVALID_COLOR,
	ERR_OCT_BIG,
	ERR_INVALID_OCT,
} type;

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

bool	check_is_closed(char* line)
{
	int	i;

	i = -1;
	while (line[++i])
	{
		if (line[i] != '1' && line[i] != ' ')
			 return false;
	}
	return true;
}

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

bool	err_print(int type)
{
	printf("Error\n");
	if (type == ERR_DUP_ELEM)
		printf("Duplicated element found\n");
	else if (type == ERR_UNKNOWN_ELEM)
		printf("Unknown element found\n");
	else if (type == ERR_INVALID_VALUE)
		printf("invalid value element found\n");
	else if (type == ERR_INVALID_KEY)
		printf("Invalid or missing key element\n");
	else if (type == ERR_INVALID_OCT)
		printf("Invalid octet\n");
	else if (type == ERR_OCT_BIG)
		printf("Invalid octet size\n");
	else if (type == ERR_INVALID_COLOR)
		printf("Invalid color foramt\n");
	else if (type == ERR_INVALID_PATH)
		printf("Invalid path\n");
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
	if (!val_size)
		return (err_print(ERR_INVALID_VALUE));
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

bool read_config(t_lex* lex, t_dyn* dyn, t_foo* config, char* file)
{
	size_t   i;
	char**   lines;

	i = 0;
	lines = (char**)dyn->buff;
	while (i < dyn->length)
	{
		if (is_config_done(config))
			break;
		if (!is_empty_line(lines[i]))
		{
			lex_init(lex, lines[i], ft_strlen(lines[i]), file);
			if (!config_readline(lines[i], config, lex))
				return false;
		}
		i++;
	}
	lex->map_start = i;
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

// bool	check_file_name(char *filename)
// {
// 	if (!filename)
// 		info()->error.err = NO_FILENAME;
// 	else if (!*filename)
// 		info()->error.err = EMPTY_FILE;
// 	else if (!ft_strncmp(ft_strrchr(filename, '.'), ".cub", 5) && ft_strlen(filename) > 4)
// 	{
// 		info()->error.err = NO_ERR;
// 		return true;
// 	}
// 	else
// 		info()->error.err = INVALID_FILENAME;
// 	return false;
// }

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

int	lex_color_octet(t_lex* lex)
{
	bool has_digit;
	int  value;

	value = 0;
	has_digit = false;
	while (ft_isdigit(lex->text[lex->pos]))
	{
		has_digit = true;
		value = value * 10 + (lex->text[lex->pos] - '0');
		if (value > 255)
			return (-1);
		lex->pos++;
	}
	if (!has_digit)
		return (-1);
	// *clr = (*clr << 8) | value;
	return (value);
}

// enum ErrType{
// 	ERR_DUP_ELEM,
// 	ERR_UNKNOWN_ELEM,
// 	ERR_INVALID_VALUE,
// 	ERR_INVALID_KEY,
// 	ERR_INVALID_COLOR,
// 	ERR_OCT_BIG,
// 	ERR_INVALID_OCT,
// } type;
//
bool	parse_last_octet(t_lex* lex, char * color, t_color* c)
{
	int octet;

	octet = lex_color_octet(lex);
	if (octet == -1)
		return (err_print(ERR_INVALID_OCT));
	*c = (*c << 8) | octet;
	lex_skip_spaces(lex);
	if (lex->pos != lex->len)
		return (err_print(ERR_INVALID_OCT));
	return (true);
}

bool	parse_color(char * color, t_color* c)
{
	int	i;
	int octet;
	t_lex lex;

	i = 0;
	lex_init(&lex, color, ft_strlen(color), "unused");
	color = 0;
	while (i < 2)
	{
		octet = lex_color_octet(&lex);
		if (octet == -1)
			return (err_print(ERR_INVALID_OCT));
		lex_skip_spaces(&lex);
		if (!lex_try_read_str(&lex, ","))
			return (err_print(ERR_INVALID_COLOR));
		lex_skip_spaces(&lex);
		*c = (*c << 8) | octet;
		++i;
	}
	return (parse_last_octet(&lex, color, c));
}

bool	validate_paths(char* path)
{
	int  i;

	i = -1;
	while(path[++i])
	{
		if (path[i + 1] != ' ' && path[i + 1])
			continue;
		if (i >= 4 && !ft_strncmp(path + i - 3,".xpm", 4)
			&& is_empty_line(path + i + 1))
		{
			path[i + 1] = '\0';
			return true;
		}
		break;
	}
	return err_print(ERR_INVALID_PATH);
}

void	get_map_size(t_foo* config, size_t end)
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
	printf("i %d ------- %d lafan dialo\n", i , end);
	config->map_width  = width;
}

bool is_(char *curr, char *next, char *prev, int i)
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

bool	log_error(char *error)
{
	printf("Error\n%s\n", error);
	return (false);
}

bool	check_map_line(char** lines, size_t j)
{
	size_t   i;

	i = 0;
	while (lines[j][i])
	{
		if (ft_strchr("0WESNCO", lines[j][i]))
		{
			if (!is_(lines[j], lines[j + 1], lines[j - 1], i))
				return (log_error("Invalid character position"));
		}
		else if (!is_valid_char(lines[j][i]))
			return (log_error("Unknown character"));
		++i;
	}
	return true;
}

bool	read_map(t_foo* config)
{
	size_t i;

	if (!check_is_closed(config->map[0]))
		return (log_error("Surrounded by Invalid character"));
	if (!check_is_closed(config->map[config->map_height - 1]))
		return (log_error("Surrounded by Invalid character"));
	i = 0;
	while (i < config->map_height)
	{
		if (!check_map_line(config->map, i))
			return (false);
		i++;
	}
	return (true);
}


bool validate_config(t_foo *config)
{
	size_t i;

	if (!is_config_done(config))
		return (false);
	i = 0;
	while (i < 4)
	{
		if (!validate_paths(config->value[i]))
			return (false);
		i++;
	}
	if (!parse_color(config->value[C], &config->c))
		return (false);
	if (!parse_color(config->value[F], &config->f))
		return (false);
	return true;
}



bool	read_file(t_dyn* dyn, t_foo* config, char* file)
{
	t_lex	lex;
	char**	lines;
	size_t	i;

	ft_bzero(&lex, sizeof(lex));
	if (!read_config(&lex, dyn, config, file))
		return (false);
	if (!validate_config(config))
		return (false);
	i = lex.map_start;
	lines = (char**)dyn->buff;
	while (i < dyn->length && is_empty_line(lines[i]))
		++i;
	if (i == dyn->length)
		return (log_error("Missing map"));
	config->map = lines + i;
	get_map_size(config, dyn->length - i);
	printf("MAP_SIZE %zux%zu\n", config->map_width,config->map_height);
	if (!read_map(config))
		return (false);
	i += config->map_height;
	while (i < dyn->length && !is_empty_line(lines[i]))
		++i;
	if (i != dyn->length)
		return (log_error("Map is not the last element"));
	return true;
}

bool parse_content(char *filename, t_game* game)
{
	t_dyn	lines;
	t_foo	config;
	int	fd;
	int	i;

	// info()->error.file = filename;
	ft_bzero(&config, sizeof(config));
	// if (!check_file_name(filename))
	// 	return false;
	fd = open_file(filename);
	load_content_from_file(fd, &lines);
	if (!read_file(&lines, &config, filename))
		exit(1);
	return (true);
}
