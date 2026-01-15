/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_parse.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aljbari <jbariali002@gmail.com>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/15 15:38:01 by aljbari           #+#    #+#             */
/*   Updated: 2026/01/15 15:38:33 by aljbari          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

bool	log_error(char *error);
void	release_file_exit(t_dyn *file, t_game *game);
bool	is_valid_char(char c);
bool	is_empty_line(char *line);
int	open_file(char *filename);
bool	check_is_closed(char *line);
bool	lex_try_read_str(t_lex *lex, char *str);
t_tok	try_read_type(t_lex *lex);
void	lex_init(t_lex *lex, char *str, size_t len);
void	lex_skip_spaces(t_lex *lex);
int	try_get_value(t_lex *lex);
bool	config_readline(t_config *config, t_lex *lex);
void	print_config(t_config *config);
bool	is_config_done(t_config *config);
bool	read_config(t_lex *lex, t_dyn *dyn, t_config *config);
void	delete_last_newline(char *line);
void	load_content_from_file(int fd, t_dyn *dyn);
int	lex_color_octet(t_lex *lex);
bool	parse_last_octet(t_lex *lex, t_color *c);
bool	parse_color(char *color, t_color *c);
bool	validate_paths(char *path);
void	get_map_size(t_config *config, size_t end);
bool	is_val(char *curr, char *next, char *prev, size_t i);
bool	check_map_line(char **lines, size_t j, t_config *config);
bool	read_map(t_config *config);
bool	validate_config(t_config *config);
bool	read_file(t_dyn *dyn, t_config *config);
bool	check_file_name(char *filename);
void	release_world(t_world *world);
t_world	build_map(t_config *config);
bool	construct_game(t_config *config, t_game *game);
bool	parse_content(char *filename, t_game *game);
