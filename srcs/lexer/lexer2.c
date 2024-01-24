/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 19:40:44 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 19:41:28 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_syntax_error(t_command *command, int i, int j, t_minishell **main)
{
	if (command[i].type && command[j].type)
	{
		if ((command[i].type == DB_RIGHT_CHEV && command[j].type == RIGHT_CHEV) \
		|| (command[i].type == DB_RIGHT_CHEV \
		&& command[j].type == DB_RIGHT_CHEV))
		{
			return (ft_error(main, \
			"minishell: syntax error near unexpected token '>'", 2));
		}
		else if ((command[i].type == DB_LEFT_CHEV \
		&& command[j].type == LEFT_CHEV) || (command[i].type == DB_LEFT_CHEV \
		&& command[j].type == DB_LEFT_CHEV))
		{
			return (ft_error(main, \
			"minishell: syntax error near unexpected token '<'", 2));
		}
		else if (command[i].type == PIPE && command[j].type == PIPE)
		{
			return (ft_error(main, \
			"minishell: syntax error near unexpected token '|'", 2));
		}
	}
	return (0);
}

int	error_nonexistent_type(t_command *command, t_minishell **main)
{
	int	error;
	int	i;
	int	j;

	i = 0;
	j = 1;
	if (command[i].type && !command[j].type && \
	(command[i].type >= LEFT_CHEV && command[i].type <= DB_LEFT_CHEV))
		return (ft_error(main, \
		"minishell: syntax error near unexpected token 'newline'", 2));
	while (command[i].type && command[j].type)
	{
		error = check_syntax_error(command, i, j, main);
		if (error)
			return (error);
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
	return (0);
}
