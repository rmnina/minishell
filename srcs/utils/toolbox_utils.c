/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   toolbox_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 05:26:30 by jdufour           #+#    #+#             */
/*   Updated: 2024/01/11 15:13:29 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	ft_count(t_command *command, int *i)
{
	int	size;

	size = 0;
	while (command[*i + size].type && command[*i + size].type == WORD)
		size++;
	return (size);
}

int	is_builtin(char *command)
{
	if (ft_strcmp(command, "cd") == 0 \
		|| ft_strcmp(command, "echo") == 0 \
		|| ft_strcmp(command, "env") == 0 \
		|| ft_strcmp(command, "exit") == 0 \
		|| ft_strcmp(command, "export") == 0 \
		|| ft_strcmp(command, "pwd") == 0 \
		|| ft_strcmp(command, "unset") == 0)
	{
		return (1);
	}
	return (0);
}

// This function creates a null t_command token. It will be added at the
// end of the array, so we can iterate on it by having an exit condition.

t_command	token_null(t_command *token, t_alloc **garbage)
{
	token->word = garb_malloc(sizeof(char), 1, garbage);
	token->word[0] = '\0';
	token->type = 0;
	return (*token);
}

void	check_spaces(t_minishell **main, int *i)
{
	if ((*main)->line[*i] == 32)
	{
		while ((*main)->line[*i] && (*main)->line[*i] == 32)
			(*i)++;
	}
}
