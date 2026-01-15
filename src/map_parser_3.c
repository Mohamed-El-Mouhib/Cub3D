/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parser_3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:47:39 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:54:07 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/map_parse.h"

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
	if (type == TOKEN_NO)
		return ("TOKEN_NO");
	if (type == TOKEN_SO)
		return ("TOKEN_SO");
	if (type == TOKEN_WE)
		return ("TOKEN_WE");
	if (type == TOKEN_EA)
		return ("TOKEN_EA");
	if (type == TOKEN_F)
		return ("TOKEN_F");
	if (type == TOKEN_C)
		return ("TOKEN_C");
	if (type == TOKEN_INVALID)
		return ("TOKEN_INVALID");
	else
		return ("TOKEN_INVALID");
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
	return (true);
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
			return (false);
	}
	return (true);
}
