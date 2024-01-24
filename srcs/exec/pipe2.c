/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:48:52 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 18:53:41 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

void	wait_pids(t_minishell **main)
{
	int	j;
	int	status;

	j = 0;
	status = 0;
	while ((*main)->pid[j] != -2)
	{
		if (waitpid((*main)->pid[j], &status, 0) == -1)
		{
			(*main)->code_status = 255;
			perror("waitpid");
			exit(255);
		}
		if (WIFEXITED(status))
			(*main)->code_status = WEXITSTATUS(status);
		j++;
	}
}

void	child_process(t_minishell **main, int *i, t_alloc **garbage)
{
	if (check_redir(main, i) == 0 && (*main)->command[*i].type != DB_LEFT_CHEV)
	{
		if ((ft_redirect(main, i, garbage)) == -1)
		{
			(*main)->code_status = 1;
			exit(EXIT_FAILURE);
		}
	}
	if (execute_builtins(main, garbage) == -1)
		execute_command(main, garbage);
	exit(EXIT_SUCCESS);
}

int	first_pipe(t_minishell **main, int *i, t_alloc **garbage)
{
	if (pipe((*main)->fd) == -1)
		return (-1);
	(*main)->nb_cmd++;
	(*main)->pid[(*main)->nb_cmd - 1] = fork();
	if ((*main)->pid[(*main)->nb_cmd - 1] == -1)
		return (-1);
	if ((*main)->pid[(*main)->nb_cmd - 1] == 0)
	{
		close((*main)->fd[0]);
		dup2((*main)->fd[1], STDOUT_FILENO);
		close((*main)->fd[1]);
		child_process(main, i, garbage);
	}
	else
	{
		close((*main)->fd[1]);
		(*main)->old_fd = dup((*main)->fd[0]);
		close((*main)->fd[0]);
	}
	return (0);
}
