/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/28 21:46:00 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/29 16:57:27 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	error_quotes(t_minishell **main)
{
	int	i;

	i = 0;
	if ((*main)->line[0] == '$' && !(*main)->line[1])
		return (ft_error \
		(main, "$: command not found : No such file or directory\n", 127));
	while ((*main)->line[i])
	{
		is_in_quote((*main)->line[i], (*main)->parser);
		i++;
	}
	if ((*main)->parser->case_double == TRUE)
		return (ft_error(main, \
		"minishell: syntax error near unexpected token \"\n", 2));
	else if ((*main)->parser->case_single == TRUE)
		return (ft_error(main, \
		"minishell: syntax error near unexpected token \'\n", 2));
	(*main)->parser->case_double = FALSE;
	(*main)->parser->case_single = FALSE;
	(*main)->parser->case_quotes = FALSE;
	return (0);
}

int	wrong_use_type(t_command *command, t_minishell **main)
{
	int	i;
	int	j;

	i = 0;
	j = 1;
	while (command[j].type)
	{
		if (command[i].type >= PIPE && command[i].type <= DB_LEFT_CHEV \
		&& command[j].type >= PIPE && command[j].type <= DB_RIGHT_CHEV)
		{
			if (command[i].type != PIPE && !(command[j].type == RIGHT_CHEV \
			|| command[j].type == DB_RIGHT_CHEV))
			{
				return (write(2, \
				"minishell: syntax error near unexpected token '", \
				47), write(2, command[i].word, ft_strlen(command[i].word)), \
				ft_error(main, "'\n", 2));
			}
		}
		i++;
		j++;
	}
	return (0);
}
