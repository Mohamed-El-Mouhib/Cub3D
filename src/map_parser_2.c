/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:47:01 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:47:08 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/map_parse.h"

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
