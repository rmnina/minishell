/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/11 07:44:28 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


int handle_command_args(t_minishell **main, int *i, t_alloc **garbage)
{
	if ((*main)->command[*i].type == WORD)
		(*main)->cmd_args = create_cmd_args(main, i, garbage);
	if ((*main)->command[*i].type >= LEFT_CHEV && (*main)->command[*i].type <= DB_RIGHT_CHEV)
		return (0);
	return (1);
}

void initialize_process(t_minishell **main, int *i)
{
	if ((*main)->command[*i].type == PIPE || *i > 0)
		pipe((*main)->pipe_fd);
	(*main)->pid = fork();
	if ((*main)->pid == -1)
	{
		perror("pid");
		exit(EXIT_FAILURE);
	}
}

void execute_child_process(t_minishell **main, int *i, int *old_fd, t_alloc **garbage)
{
	if (*i > 0 && *old_fd != -1)
	{
		dup2(*old_fd, STDIN_FILENO);
		close(*old_fd);
	}
	if ((*main)->command[*i].type == PIPE)
	{
		dup2((*main)->pipe_fd[1], STDOUT_FILENO);
		close((*main)->pipe_fd[1]);
	}
	close((*main)->pipe_fd[0]);
	if (execute_builtins(main, garbage) == -1)
		execute_non_builtin(main, garbage);
	exit(EXIT_SUCCESS);
}

void handle_parent_process(t_minishell **main, int *i, int *old_fd, int *status)
{
	if (*i > 0 && *old_fd != -1)
	{
		close(*old_fd);
	}
	if ((*main)->command[*i].type == PIPE)
	{
		*old_fd = (*main)->pipe_fd[0];
		close((*main)->pipe_fd[1]);
	}
	else
	{
		close((*main)->pipe_fd[0]);
		close((*main)->pipe_fd[1]);
	}
	waitpid((*main)->pid, status, 0);
	if (WIFEXITED(*status))
		(*main)->code_status = WEXITSTATUS(*status);
}

int ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
{
	int		status;
	int		old_fd;

	status = 0;
	old_fd = -1;
	while ((*main)->command[*i].type != 0)
	{
		if (!handle_command_args(main, i, garbage))
			break ;
		initialize_process(main, i);
		if ((*main)->pid == 0)
			execute_child_process(main, i, &old_fd, garbage);
		else
			handle_parent_process(main, i, &old_fd, &status);
		(*i)++;
	}
	if (old_fd != -1)
		close(old_fd);
	return (1);
}
