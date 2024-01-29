/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:56:52 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/29 01:05:50 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/minishell.h"

void	child_process(t_minishell **main, int *i, t_alloc **garbage)
{
	if (check_redir(main, i) == 0 && (*main)->command[*i].type != DB_LEFT_CHEV)
	{
		if ((ft_redirect(main, i, garbage)) == -1)
		{
			(*main)->code_status = 1;
			free_garbage(garbage);
			exit(EXIT_FAILURE);
		}
	}
	if (execute_builtins(main, garbage) == -1)
		execute_command(main, garbage);
	free_garbage(garbage);
	exit(EXIT_SUCCESS);
}

int	pipex_loop(t_minishell **main, int *i, t_alloc **garbage)
{
	if (is_heredoc(main, i))
	{
		(*main)->heredoc = ft_heredoc(main, i, garbage);
		(*main)->heredoc_used = 1;
		if ((*main)->heredoc == -1)
			return (-1);
	}
	if (is_first_pipe(main, i))
		first_pipe(main, i, garbage);
	else
		middle_pipe(main, i, garbage);
	if (check_redir(main, i) == 0)
	{
		while ((*main)->command[*i].type && (*main)->command[*i].type != PIPE)
			(*i)++;
	}
	(*i)++;
	(*main)->cmd_args = create_cmd_args(main, i, garbage);
	return ((*main)->heredoc = 0);
}

int	ft_pipex(t_minishell **main, int *i, t_alloc **garbage)
{
	int	fd_stdin;

	init_process_signal();
	while ((*main)->nb_cmd < (*main)->total_cmd - 1)
	{
		if (pipex_loop(main, i, garbage) == -1)
			return (-1);
	}
	if (is_heredoc(main, i))
	{
		init_heredoc_pipe_signal();
		(*main)->heredoc = ft_heredoc(main, i, garbage);
		if ((*main)->heredoc == -1)
			return (-1);
		(*main)->heredoc_used = 1;
		init_process_signal();
	}
	last_pipe(main, i, garbage);
	if ((*main)->heredoc_used)
	{
		fd_stdin = open("/dev/tty", O_RDONLY);
		dup2(fd_stdin, STDIN_FILENO);
		close(fd_stdin);
	}
	wait_pids(main);
	restore_fds(main);
	return (0);
}
