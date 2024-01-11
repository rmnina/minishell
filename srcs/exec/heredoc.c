/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdufour <jdufour@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 16:22:53 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/11 15:15:14 by jdufour          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_heredoc_args(t_minishell **main, int i, t_alloc **garbage)
{
	char	*new_argv[2];
	
	if (is_builtin((*main)->cmd_args[i]))
		execute_builtins(main, garbage);
	else
	{
		(*main)->path = find_command_path((*main)->cmd_args[i], garbage);
		if (!(*main)->path)
			exit(EXIT_FAILURE);
		new_argv[0] = ft_strdup((*main)->cmd_args[i], garbage);
		if (!new_argv[0])
			exit (EXIT_FAILURE);
		new_argv[1] = NULL;
		execve((*main)->path, new_argv, (*main)->envp);
		exit(EXIT_FAILURE);
	}
}

void	heredoc_parent(t_minishell **main)
{
	int	status;
	
	status = 0;
	close((*main)->pipe_fd[0]);
	close((*main)->pipe_fd[1]);
	if (g_sigstatus == 130)
		exit(130);
	waitpid((*main)->pid, &status, 0);
	if (WIFEXITED(status))
	{
		(*main)->code_status = WEXITSTATUS(status);
		if ((*main)->code_status == SPECIAL_EXIT_CODE)
			exit((*main)->code_status);
	}
}

void	heredoc_child(t_minishell **main, t_alloc **garbage)
{
	(*main)->path = NULL;
	if (dup2((*main)->pipe_fd[0], STDIN_FILENO) == -1)
		exit(EXIT_FAILURE);
	close((*main)->pipe_fd[0]);
	if (ft_strcmp((*main)->cmd_args[0], "<<"))
		ft_heredoc_args(main, 0, garbage);
	else
		ft_heredoc_args(main, 2, garbage);
	exit(EXIT_SUCCESS);
}


void	init_heredoc(t_minishell **main, int *i, t_alloc **garbage)
{
	int	j;

	j = 0;
	(*main)->h_delimiter = get_delimiter(main, i, garbage);
	while ((*main)->h_delimiter[j])
	{	
		pipe((*main)->pipe_fd);
		read_add(main, &j, garbage);
		j++;
	}
	close((*main)->pipe_fd[1]);
}

void	ft_heredoc(t_minishell **main, int *i, t_alloc **garbage)
{
	init_heredoc(main, i, garbage);
	init_process_signal();
	(*main)->pid = fork();
	if ((*main)->pid == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	else if ((*main)->pid == 0)
	{
		if ((*main)->cmd_args == NULL)
			(*main)->cmd_args = create_cmd_args(main, i, garbage);
		heredoc_child(main, garbage);
		exit(EXIT_SUCCESS);
	}
	else
		heredoc_parent(main);
}