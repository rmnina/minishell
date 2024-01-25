/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 22:13:40 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

int	is_heredoc(t_minishell **main, int *i)
{
	int	j;

	j = 0;
	while ((*main)->command[*i + j].type \
	&& (*main)->command[*i + j].type != PIPE)
	{
		if ((*main)->command[*i + j].type == DB_LEFT_CHEV)
			return (1);
		j++;
	}
	return (0);
}

int	is_first_pipe(t_minishell **main, int *i)
{
	int	count;

	count = 0;
	while (count < *i)
	{
		if ((*main)->command[*i].type == PIPE)
			return (0);
		count++;
	}
	return (1);
}

int	is_last_pipe(t_minishell **main, int *i)
{
	int	count;

	count = 0;
	while ((*main)->command[*i + count].type)
	{
		if ((*main)->command[*i + count].type == PIPE)
			return (0);
		count++;
	}
	return (1);
}

int	will_be_piped(t_minishell **main, int *i)
{
	int	j;

	j = 0;
	while ((*main)->command[*i + j].type && \
	(*main)->command[*i + j].type != PIPE)
		j++;
	if ((*main)->command[*i + j].type == PIPE)
		return (1);
	return (0);
}

int	check_redir(t_minishell **main, int *i)
{
	int	j;

	j = 0;
	while ((*main)->command[*i + j].type && \
	(*main)->command[*i + j].type != PIPE)
	{
		if ((*main)->command[*i + j].type >= LEFT_CHEV \
		&& (*main)->command[*i + j].type <= DB_LEFT_CHEV)
			return (j);
		j++;
	}
	return (-1);
}