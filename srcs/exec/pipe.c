/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 19:46:28 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	init_pids(t_minishell **main, t_alloc **garbage)
{
	int	count;
	int	i;
	int	j;

	count = 1;
	i = 0;
	j = 0;
	while ((*main)->command[j].type)
	{
		if ((*main)->command[j].type == PIPE)
			count++;
		j++;
	}
	if (count == 0)
		return (0);
	(*main)->pid = garb_malloc(sizeof(pid_t), count + 1, PARSING, garbage);
	if ((*main)->pid == NULL)
		return (-1);
	while (i < count)
	{
		(*main)->pid[i] = 1;
		i++;
	}
	(*main)->pid[i] = -2;
	return (count);
}

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
