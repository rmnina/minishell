/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_process.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/25 16:20:20 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_builtins(t_minishell **main, t_alloc **garbage)
{
	if ((*main)->cmd_args[0] == NULL)
		return (0);
	if (ft_strcmp((*main)->cmd_args[0], "cd") == 0
		&& ft_strlen((*main)->cmd_args[0]) == ft_strlen("cd"))
		return (ft_cd(main, garbage));
	if (ft_strcmp((*main)->cmd_args[0], "echo") == 0
		&& ft_strlen((*main)->cmd_args[0]) == ft_strlen("echo"))
		return (ft_echo(main));
	if (ft_strcmp((*main)->cmd_args[0], "env") == 0
		&& ft_strlen((*main)->cmd_args[0]) == ft_strlen("env"))
		return (ft_env(main));
	if (ft_strcmp((*main)->cmd_args[0], "exit") == 0
		&& ft_strlen((*main)->cmd_args[0]) == ft_strlen("exit"))
		return (ft_exit(main, garbage));
	if (ft_strcmp((*main)->cmd_args[0], "export") == 0
		&& ft_strlen((*main)->cmd_args[0]) == ft_strlen("export"))
		return (ft_export(main, garbage));
	if (ft_strcmp((*main)->cmd_args[0], "pwd") == 0
		&& ft_strlen((*main)->cmd_args[0]) == ft_strlen("pwd"))
		return (ft_pwd(main));
	if (ft_strcmp((*main)->cmd_args[0], "unset") == 0
		&& ft_strlen((*main)->cmd_args[0]) == ft_strlen("unset"))
		return (ft_unset(main, (*main)->cmd_args + 1));
	return (-1);
}

void	execute_command(t_minishell **main, t_alloc **garbage)
{
	(*main)->path = NULL;
	if (!(*main)->cmd_args)
	{
		perror("Error creating command args");
		exit(EXIT_FAILURE);
	}
	(*main)->path = find_command_path(main, (*main)->cmd_args[0], garbage);
	if (!(*main)->path)
	{
		ft_putstr_fd("Command '", 2);
		if ((*main)->cmd_args[0])
			ft_putstr_fd((*main)->cmd_args[0], 2);
		ft_putstr_fd("' not found\n", 2);
		free_small_garb(garbage);
		exit(127);
	}
	execve((*main)->path, (*main)->cmd_args, (*main)->envp);
	free_small_garb(garbage);
	perror("execve");
	exit(EXIT_FAILURE);
}

int	execute_non_builtin(t_minishell **main, t_alloc **garbage)
{
	pid_t	pid;
	int		status;

	status = 0;
	init_process_signal();
	pid = fork();
	if (pid == -1)
	{
		perror ("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		execute_command(main, garbage);
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			(*main)->code_status = WEXITSTATUS(status);
	}
	return (-1);
}

void	init_redirect(t_minishell **main, int *i, t_alloc **garbage)
{
	int	pid;
	int	status;

	status = 0;
	pid = fork();
	if (!pid)
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
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		(*main)->code_status = WEXITSTATUS(status);	
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
