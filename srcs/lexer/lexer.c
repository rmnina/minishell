/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 02:11:36 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/14 04:47:07 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_error(t_minishell **main, char *str, int i)
{
	ft_putstr_fd(str, 2);
	(*main)->code_status = i;
	return (-1);
}

// This function checks for lexer errors in quotes. It uses the 
// booleand set by is_in_quotes(), and returns an error if the 
// latter indicates the presence of an unmatch opening quote
// without a closing quote.

int	error_quotes(t_minishell **main)
{
	int	i;

	i = 0;
	if ((*main)->line[0] == '$' && !(*main)->line[1])
		return (ft_error(main, "Command not found : No such file or directory", 127));
	while ((*main)->line[i])
	{
		is_in_quote((*main)->line[i], (*main)->parser);
		i++;
	}
	if ((*main)->parser->case_double == TRUE)
		return (ft_error(main, "minishell: syntax error near unexpected token \"", 2));
	else if ((*main)->parser->case_single == TRUE)
		return (ft_error(main, "minishell: syntax error near unexpected token \'", 2));
	(*main)->parser->case_double = FALSE;
	(*main)->parser->case_single = FALSE;
	(*main)->parser->case_quotes = FALSE;
	return (0);
}

// The 2 following functions checks for parsing error once the array
// of structures has been created. The first one verifies the syntax 
// of commands with pipes or redirections ; the seconds only focuses
// on the number of > or <.

int	error_use_types(t_command *command, t_minishell **main)
{
	int	i;

	i = 0;
	if (!command[0].type)
		return (-1);
	if (command[0].type == PIPE)
		return \
		(ft_error(main, "minishell : syntax error near unexpected token '|'", 2));
	while (command[i].type)
		i++;
	i--;
	if (command[i].type == RIGHT_CHEV || command[i].type == LEFT_CHEV \
	|| command[i].type == DB_RIGHT_CHEV || command[i].type == DB_LEFT_CHEV)
		return \
		(ft_error(main, "minishell : syntax error near unexpected token 'newline'", 2));
	return (0);
}

int	error_nonexistent_type(t_command *command, t_minishell **main)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (command[i].type && command[j].type)
	{
		if (command[i].type && command[j].type && \
		((command[i].type == DB_RIGHT_CHEV && command[j].type == RIGHT_CHEV) || \
		(command[i].type == DB_RIGHT_CHEV && command[j].type == DB_RIGHT_CHEV)))
			return \
			(ft_error(main, "minishell: syntax error near unexpected token '>'", 2));
		else if (command[i].type && command[j].type && \
		((command[i].type == DB_LEFT_CHEV && command[j].type == LEFT_CHEV) || \
		(command[i].type == DB_LEFT_CHEV && command[j].type == DB_LEFT_CHEV)))
			return \
			(ft_error(main, "minishell: syntax error near unexpected token '<'", 2));
		else if (command[i].type && command[j].type && \
		((command[i].type == PIPE && command[j].type == PIPE)))
			return \
			(ft_error(main, "minishell: syntax error near unexpected token '|'", 2));
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
