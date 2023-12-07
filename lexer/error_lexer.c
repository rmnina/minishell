/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_lexer.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 02:11:36 by jdufour           #+#    #+#             */
/*   Updated: 2023/12/07 15:44:22 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// This function checks for lexer errors in quotes. It uses the 
// booleand set by is_in_quotes(), and returns an error if the 
// latter indicates the presence of an unmatch opening quote
// without a closing quote.

void	error_quotes(char *line, t_quotes *quotes)
{
	int	i;

	i = 0;
	while (line[i])
	{
		is_in_quote(line[i], quotes);
		i++;
	}
	if (quotes->case_double == TRUE)
	{
		printf("minishell: syntax error near unexpected token \"\n");
		exit(2);
	}
	else if (quotes->case_single == TRUE)
	{
		printf("minishell: syntax error near unexpected token \'\n");
		exit(2);
	}
}

// The 2 following functions checks for parsing error once the array
// of structures has been created. The first one verifies the syntax 
// of commands with pipes or redirections ; the seconds only focuses
// on the number of > or <.

void	error_use_types(t_command *command)
{
	int	i;

	i = 0;
	if (!command[0].type)
		exit(1);
	if (command[0].type == PIPE)
	{
		printf("minishell : syntax error near unexpected token '|'");
		ft_free_command(command);
		exit(2);
	}
	while (command[i].type)
		i++;
	i--;
	if (command[i].type == RIGHT_CHEV || command[i].type == LEFT_CHEV \
	|| command[i].type == DB_RIGHT_CHEV || command[i].type == DB_LEFT_CHEV)
	{
		printf("minishell : syntax error near unexpected token 'newline'\n");
		ft_free_command(command);
		exit(2);
	}
}

void	error_nonexistent_type(t_command *command)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (command[i].type && command[j].type)
	{
		if (command[i].type == 5 && command[j].type == 3)
		{
			printf("minishell: syntax error near unexpected token '<'\n");
			exit(2);
		}
		else if (command[i].type == 6 && command[j].type == 4)
		{
			printf("minishell: syntax error near unexpected token '>'\n");
			exit(2);
		}
		i++;
		j++;
	}
}


void	ft_error_lexer(t_command *command)
{
	error_nonexistent_type(command);
	error_use_types(command);
}