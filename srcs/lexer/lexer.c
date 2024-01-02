/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 02:11:36 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/02 13:28:48 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_error(char *str)
{
	printf("%s\n", str);
	return (-1);
}

// This function checks for lexer errors in quotes. It uses the 
// booleand set by is_in_quotes(), and returns an error if the 
// latter indicates the presence of an unmatch opening quote
// without a closing quote.

int	error_quotes(char *line, t_quotes *quotes)
{
	int	i;

	i = 0;
	while (line[i])
	{
		is_in_quote(line[i], quotes);
		i++;
	}
	if (quotes->case_double == TRUE)
		return (ft_error("minishell: syntax error near unexpected token \""));
	else if (quotes->case_single == TRUE)
		return (ft_error("minishell: syntax error near unexpected token \'"));
	return (0);
}

// The 2 following functions checks for parsing error once the array
// of structures has been created. The first one verifies the syntax 
// of commands with pipes or redirections ; the seconds only focuses
// on the number of > or <.

int	error_use_types(t_command *command)
{
	int	i;

	i = 0;
	if (!command[0].type)
		return (-1);
	if (command[0].type == PIPE)
		return \
		(ft_error("minishell : syntax error near unexpected token '|'"));
	while (command[i].type)
		i++;
	i--;
	if (command[i].type == RIGHT_CHEV || command[i].type == LEFT_CHEV \
	|| command[i].type == DB_RIGHT_CHEV || command[i].type == DB_LEFT_CHEV)
		return \
		(ft_error("minishell : syntax error near unexpected token 'newline'"));
	return (0);
}

int	error_nonexistent_type(t_command *command)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (command[i].type && command[j].type)
	{
		if (command[i].type && command[j].type && \
		((command[i].type == 5 && command[j].type == 4) || \
		(command[i].type == 5 && command[j].type == 5)))
			return \
			(ft_error("minishell: syntax error near unexpected token '>'"));
		else if (command[i].type && command[j].type && \
		((command[i].type == 6 && command[j].type == 3) || \
		(command[i].type == 6 && command[j].type == 6)))
			return \
			(ft_error("minishell: syntax error near unexpected token '<'"));
		i++;
		j++;
	}
	return (0);
}

int	ft_error_lexer(t_command *command)
{
	if (error_nonexistent_type(command) == -1 \
	|| error_use_types(command) == -1)
		return (-1);
	return (0);
}
