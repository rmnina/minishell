/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:35:46 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/24 18:47:37 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	process_child(t_minishell **main, int *i, t_alloc **garbage)
{
	while ((*main)->command[*i].type)
	{
		if ((*main)->command[*i].type == DB_LEFT_CHEV)
		{
			ft_heredoc(main, i, garbage);
			if ((*main)->cmd_args[0] == NULL)
				(*main)->cmd_args = create_cmd_args(main, i, garbage);
		}
		if (ft_redirect(main, i, garbage) == -1)
		{
			(*main)->code_status = 1;
			exit(EXIT_FAILURE);
		}
	}
	if (execute_builtins(main, garbage) == -1)
		execute_command(main, garbage);
	exit(EXIT_SUCCESS);
}

void	init_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	int	pid;
	int	status;

	status = 0;
	pid = fork();
	if (pid == 0)
	{
		process_child(main, i, garbage);
	}
	else if (pid > 0)
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			(*main)->code_status = WEXITSTATUS(status);
	}
	else
	{
		perror("fork");
		(*main)->code_status = 1;
	}
}

void	fork_heredoc(t_minishell **main, int *i, t_alloc **garbage)
{
	int	pid;
	int	status;

	status = 0;
	pid = fork();
	if (!pid)
	{
		if (ft_heredoc(main, i, garbage) != -1)
		{
			if (execute_builtins(main, garbage) == -1)
				execute_command(main, garbage);
		}
		else
		{
			(*main)->code_status = 1;
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
	}
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*main)->code_status = WEXITSTATUS(status);
}

void	handle_command(t_minishell **main, t_alloc **garbage)
{
	int			i;

	i = 0;
	(*main)->cmd_args = NULL;
	(*main)->nb_cmd = 0;
	(*main)->command = ft_parsing(main, garbage);
	if ((*main)->command == NULL)
		return ;
	(*main)->cmd_args = create_cmd_args(main, &i, garbage);
	if (will_be_piped(main, &i))
	{
		(*main)->total_cmd = init_pids(main, garbage);
		ft_pipex(main, &i, garbage);
	}
	else if (check_redir(main, &i) != -1)
		init_redirect(main, &i, garbage);
	else
	{
		if (execute_builtins(main, garbage) == -1)
			execute_non_builtin(main, garbage);
	}
	unlink((*main)->tmp_filename);
}
