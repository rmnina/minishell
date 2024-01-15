/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: juandrie <juandrie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 12:18:22 by juandrie          #+#    #+#             */
/*   Updated: 2024/01/15 15:55:33 by juandrie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	execute_builtins(t_minishell **main, t_alloc **garbage)
{
	if ((*main)->cmd_args[0] == NULL)
		return (0);
	if (ft_strcmp((*main)->cmd_args[0], "cd") == 0
		&& ft_strlen((*main)->cmd_args[0]) == ft_strlen("cd"))
		return (ft_cd(main));
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

int	execute_command(t_minishell **main, t_alloc **garbage)
{
	(*main)->path = NULL;
	if (!(*main)->cmd_args)
	{
		perror("Error creating command args");
		exit(EXIT_FAILURE);
	}
	//printf("cmd_args : %s\n", (*main)->cmd_args[0]);
	(*main)->path = find_command_path((*main)->cmd_args[0], garbage);
	//printf("path : %s\n", (*main)->path);
	if (!(*main)->path)
	{
		perror("Command not found");
		exit(127);
	}
	//printf("Executing command: %s, path : %s\n", (*main)->path, (*main)->cmd_args[0]);
	execve((*main)->path, (*main)->cmd_args, (*main)->envp);
	perror("execve");
	//exit(EXIT_FAILURE);
	return (-1);
}

int	execute_non_builtin(t_minishell **main, t_alloc **garbage)
{
	pid_t	pid;
	int		status;

	status = 0;
	//init_process_signal();
	pid = fork();
	if (pid == -1)
	{
		perror ("fork");
		exit(EXIT_FAILURE);
	}
	else if (pid == 0)
	{
		init_sigquit();
		execute_command(main, garbage);
		exit(EXIT_FAILURE);
	}
	else
	{
		process_prompt();
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			(*main)->code_status = WEXITSTATUS(status);
	}
	return (-1);
}

char	**create_cmd_args(t_minishell **main, int *i, t_alloc **garbage)
{
	char	**cmd_args;
	int		num_args;
	int		j;

	j = 0;
	cmd_args = NULL;
	num_args = ft_count((*main)->command, i);
	cmd_args = garb_malloc(sizeof(char *), num_args + 1, garbage);
	if (!cmd_args)
		return (NULL);
	while ((*main)->command[*i].type == WORD)
	{
		cmd_args[j] = ft_strjoin(cmd_args[j], (*main)->command[*i].word, garbage);
		if (!cmd_args[j])
			return (NULL);
		*i += 1;
		j++;
	}
	cmd_args[j] = NULL;
	return (cmd_args);
}

void	handle_command(t_minishell **main, t_alloc **garbage)
{
	int			i;
	int			exec;

	i = 0;
	exec = 0;
	(*main)->cmd_args = NULL;
	(*main)->command = ft_parsing(main, garbage);
	if ((*main)->command == NULL)
		return ;
	while ((*main)->command[i].type != 0)
	{
		if ((*main)->command[i].type == WORD)
			(*main)->cmd_args = create_cmd_args(main, &i, garbage);
		if ((*main)->command[i].type >= PIPE)
			exec += ft_pipex(main, &i, garbage);
		else if ((*main)->cmd_args != NULL && exec == 0)
		{
			if (execute_builtins(main, garbage) == -1)
				execute_non_builtin(main, garbage);
		}
		else
			i++;
	}
}