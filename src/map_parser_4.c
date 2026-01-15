/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser_4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:48:08 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:48:09 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/map_parse.h"

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
				return (false);
		}
		else if (is_config_done(config))
			break ;
		i++;
	}
	lex->map_start = i;
	return (true);
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
