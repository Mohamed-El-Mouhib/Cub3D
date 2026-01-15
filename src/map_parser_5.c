/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser_5.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:48:30 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:52:02 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/map_parse.h"

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
	return (true);
}
