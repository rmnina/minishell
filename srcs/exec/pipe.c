/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 12:20:25 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/15 17:42:09 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	next_is_pipe(t_minishell **main, int *i)
{
	int	j;

	if ((*main)->command[*i].type == PIPE)
		return (1);
	j = *i;
	while ((*main)->command[*i + j].type == WORD)
		j++;
	if ((*main)->command[*i + j].type == PIPE)
		return (1);
	return (0);
}

void	handle_command_args(t_minishell **main, int *i, t_alloc **garbage)
{
	(*main)->redir = 0;
	(*main)->cmd_args = create_cmd_args(main, i, garbage);
	if ((*main)->command[*i].type >= LEFT_CHEV && (*main)->command[*i].type <= DB_LEFT_CHEV)
		(*main)->redir = 1;
}

void	initialize_process(t_minishell **main, int *i)
{
	(void)i;
	//init_process_signal();
	pipe((*main)->fd);
printf(" Dans initialize pour pipe main fd : fd[1]: %d, fd[0]: %d\n", (*main)->fd[1], (*main)->fd[0]);	
	pipe((*main)->com);
	printf(" Dans initialize pour pipe main com : fd[1]: %d, fd[0]: %d\n", (*main)->fd[1], (*main)->fd[0]);
	(*main)->pid = fork();
	if ((*main)->pid == -1)
	{
		perror("pid");
		exit(EXIT_FAILURE);
	}
}

void	execute_child_process(t_minishell **main, int *i, t_alloc **garbage)
{
	close((*main)->fd[0]);
	if ((*main)->com[0] != -1)
		close((*main)->com[0]);
	if ((*main)->old_fd != -1)
	{
		dup2((*main)->old_fd, STDIN_FILENO);
		printf(" Dans execute child dans if ((*main)->old_fd != -1) : fd[1]: %d, fd[0]: %d\n", (*main)->fd[1], (*main)->fd[0]);
		close((*main)->old_fd);
	}
	if ((*main)->command[*i].type == PIPE)
	{
		close((*main)->fd[0]);
		dup2((*main)->fd[1], STDOUT_FILENO);
		printf(" Dans execute child dans IF PIPE : fd[1]: %d, fd[0]: %d\n", (*main)->fd[1], (*main)->fd[0]);
		close((*main)->fd[1]);
	}
	if ((*main)->command[*i].type == DB_LEFT_CHEV)
	{
		close((*main)->fd[1]);
		dup2((*main)->fd[0], STDIN_FILENO);
		printf(" Dans execute child dans IF DB LEFT CHEV : fd[1]: %d, fd[0]: %d\n", (*main)->fd[1], (*main)->fd[0]);
		close((*main)->fd[0]);
	}
	if ((*main)->redir)
		(*main)->redir = ft_redirect(main, i, garbage);
	if ((*main)->com[1] != -1)
	{
		write((*main)->com[1], i, sizeof(*i));
		close((*main)->com[1]);
	}
	if ((*main)->redir == -1)
		exit(EXIT_FAILURE);
	if (execute_builtins(main, garbage) == -1)
	{
		if (execute_command(main, garbage) == -1)
			exit (EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
}

void	handle_parent_process(t_minishell **main, int *i, int *status)
{
	if ((*main)->com[1] != -1)
		close((*main)->com[1]);
	if ((*main)->fd[1] != -1)
		close((*main)->fd[1]);
	if ((*main)->old_fd != -1)
		close((*main)->old_fd);
	(*main)->old_fd = (*main)->fd[0];
	waitpid((*main)->pid, status, 0);
	if ((*main)->com[0] != -1)
	{
		read((*main)->com[0], i, sizeof(*i));
		close((*main)->com[0]);
	}
	if (WIFEXITED(*status))
		(*main)->code_status = WEXITSTATUS(*status);
}


int 	ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
{
	int		status;

	status = 0;
	handle_command_args(main, i, garbage);
	if (is_builtin((*main)->command[0].word) && (*main)->command[*i].type == 0)
	{
		execute_builtins(main, garbage);
		return (0);
	}
	initialize_process(main, i);
	if ((*main)->pid == 0)
		execute_child_process(main, i, garbage);
	else
		handle_parent_process(main, i, &status);
	(*i)++;
	if ((*main)->command[*i].type != 0)
		ft_pipex(main, i, garbage);
	if ((*main)->old_fd != -1)
		close((*main)->old_fd);
	if ((*main)->filefd != -1)
		close((*main)->filefd);
	return (1);
}
