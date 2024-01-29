/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 02:11:36 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/28 21:59:22 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_use_types(t_command *command, t_minishell **main)
{
	int	i;

	i = 0;
	if (!command[0].type)
		return (-1);
	if (command[0].type == PIPE)
		return \
		(ft_error(main, \
		"minishell : syntax error near unexpected token '|'\n", 2));
	while (command[i].type)
		i++;
	i--;
	if (command[i].type == RIGHT_CHEV || command[i].type == LEFT_CHEV \
	|| command[i].type == DB_RIGHT_CHEV || command[i].type == DB_LEFT_CHEV)
		return \
		(ft_error(main, \
		"minishell : syntax error near unexpected token 'newline'\n", 2));
	if (command[i].type == PIPE)
		return \
		(ft_error(main, \
		"minishell : syntax error : unexpected end of file\n", 2));
	return (0);
}

int	error_chev(t_command *command, int *i, int *j, t_minishell **main)
{
	if (command[*i].type && !command[*j].type \
	&& (command[*i].type >= LEFT_CHEV && command[*i].type <= DB_LEFT_CHEV))
		return \
		(ft_error(main, \
		"minishell: syntax error near unexpected token 'newline'\n", 2));
	if (command[*i].type && command[*j].type && \
	((command[*i].type == DB_RIGHT_CHEV && command[*j].type == RIGHT_CHEV) || \
	(command[*i].type == DB_RIGHT_CHEV && command[*j].type == DB_RIGHT_CHEV)))
		return \
		(ft_error(main, \
		"minishell: syntax error near unexpected token '>'\n", 2));
	else if (command[*i].type && command[*j].type && \
	((command[*i].type == DB_LEFT_CHEV && command[*j].type == LEFT_CHEV) || \
	(command[*i].type == DB_LEFT_CHEV && command[*j].type == DB_LEFT_CHEV)))
		return \
		(ft_error(main, \
		"minishell: syntax error near unexpected token '<'\n", 2));
	else if ((command[*i].type && command[*j].type && \
	command[*i].type == RIGHT_CHEV && command[*j].type == LEFT_CHEV))
		return \
		(ft_error(main, \
		"minishell: syntax error near unexpected token '<'\n", 2));
	return (0);
}

int	error_nonexistent_type(t_command *command, t_minishell **main)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	if (command[i].type && !command[j].type && \
	(command[i].type >= LEFT_CHEV && command[i].type <= DB_LEFT_CHEV))
		return \
		(ft_error(main, \
		"minishell: syntax error near unexpected token 'newline'\n", 2));
	while (command[i].type && command[j].type)
	{
		if (error_chev(command, &i, &j, main) == -1)
			return (-1);
		else if (command[i].type && command[j].type && \
		((command[i].type == PIPE && command[j].type == PIPE)))
			return \
			(ft_error(main, \
			"minishell: syntax error near unexpected token '|'\n", 2));
		i++;
		j++;
	}
	return (0);
}

int	ft_error_lexer(t_command *command, t_minishell **main)
{
	if (error_nonexistent_type(command, main) == -1 \
	|| error_use_types(command, main) == -1)
		return (-1);
	if (wrong_use_type(command, main) == -1)
		return (-1);
	return (0);
}